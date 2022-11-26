#include "cpu_lbvh.hpp"
#include <map>

namespace specter {


// Implementation
static void assignInternalNode(const int idx, InternalNode* internalNodes) {
	internalNodes[idx].nodeIdx = idx;
	internalNodes[idx].leftIdx = -1;
	internalNodes[idx].rightIdx = -1;
	internalNodes[idx].parentIdx = -1;
	internalNodes[idx].childLeaf = 0;
}

static void assignLeafNode(const int idx, LeafNode* leafNodes, PrimitiveIdentifier* pIds) {
	leafNodes[idx].parentIdx = -1;
	leafNodes[idx].leafIdx = pIds[idx].objectId;
}

static void depth_first_traversal(InternalNode* in, int nodeIdx, std::map<int, bool>& existsmap, int rdepth) {
	existsmap.try_emplace(nodeIdx, true);
	if (in[nodeIdx].leftIdx != -1) {
		if (!(in[nodeIdx].childLeaf & 1)) {
			depth_first_traversal(in, in[nodeIdx].leftIdx, existsmap, rdepth + 1);
		}
	}
	if (in[nodeIdx].rightIdx != -1) {
		if (in[nodeIdx].childLeaf < 2) {
			depth_first_traversal(in, in[nodeIdx].rightIdx, existsmap, rdepth + 1);
		}
	}
}

void CPU_LBVH::build(std::shared_ptr<Model>& model) {
	nTriangles = model->GetTriangleCount();
	internalNodes.resize(nTriangles - 1);
	leafNodes.resize(nTriangles);
	output_aabbs.resize(nTriangles - 1);
	
	// For now,
	aabbs.resize(nTriangles);

	std::vector<PrimitiveIdentifier> pIds(nTriangles);
	
	const AxisAlignedBoundingBox scene_box = model->computeBoundingBox();

	for (int i = 0; i < nTriangles; ++i) {
		aabbs[i] = constructAABBFromPaddedTriangle(
			model->GetVertex(model->GetFace(i * 3).p),
			model->GetVertex(model->GetFace(i * 3 + 1).p),
			model->GetVertex(model->GetFace(i * 3 + 2).p));

		computeMortonCode(pIds.data(), aabbs.data(), i, scene_box.min, scene_box.max);
	}
	
	// Sort primitives according to their morton codes
	radixsort(pIds.data(), nTriangles);

	for (int i = 0; i < nTriangles - 1; ++i) {
		assignInternalNode(i, internalNodes.data());
		assignLeafNode(i, leafNodes.data(), pIds.data());
	}
	// Assign the final leaf node [this was not accounted for in the loop]
	assignLeafNode(nTriangles - 1, leafNodes.data(), pIds.data());
	
	// Generate the hierarchy sequentially. however, on the GPU this will be done in parallel.
	for (int i = 0; i < nTriangles - 1; ++i) {
		generateHierarchy(i, nTriangles, pIds.data());
	}

	generateBV(aabbs.data(), nTriangles);
}

bool CPU_LBVH::traverse(const Model* model, const Ray& ray, Intersection& intersection) const {
	const int rootIdx = GetRootIndex();
	traverse_Rec(model, rootIdx, ray, intersection);
	return intersection.isValid();
}

void CPU_LBVH::traverse_Rec(const Model* model, const int nodeIdx, const Ray& ray, Intersection& its) const {

	auto handleIntersection = [](const Model* model, const Ray& ray, const int tri_index, Intersection& its)
	{
		float u, v, t = std::numeric_limits<float>::max();
		model->rayIntersection(ray, tri_index, u, v, t);
		if (its.t > t && t > 0.f) {
			its.t = t;
			its.u = u;
			its.v = v;
			auto meshIndex = model->GetMeshIndexFromFace(tri_index);
			its.mat_ptr = model->GetMaterial(meshIndex);
			its.n = model->GetNormal(model->GetFace(tri_index * 3).n);
			its.f = tri_index;
			its.p = ray.o + its.t * ray.d;
		}
	};

	// Traverse hierarchy
	// Left child
	if (internalNodes[nodeIdx].leftIdx != -1) {
		const int c = internalNodes[nodeIdx].childLeaf;
		if (c == 1 || c == 3) {
			// Leaf
			handleIntersection(model, ray, internalNodes[nodeIdx].leftIdx, its);
		} else {
			// Internal
			int leftIdx = internalNodes[nodeIdx].leftIdx;
			float near, far;
			if (output_aabbs[leftIdx].rayIntersect(ray, near, far)) {
				traverse_Rec(model, leftIdx, ray, its);
			}
		}
	}
	// Right child
	if (internalNodes[nodeIdx].rightIdx != -1) {
		const int c = internalNodes[nodeIdx].childLeaf;
		if (c == 2 || c == 3) {
			// Leaf
			handleIntersection(model, ray, internalNodes[nodeIdx].rightIdx, its);
		} else {
			// Internal
			int rightIdx = internalNodes[nodeIdx].rightIdx;
			float near, far;
			if (output_aabbs[rightIdx].rayIntersect(ray, near, far)) {
				traverse_Rec(model, rightIdx, ray, its);
			}
		}
	}
}

static int clz2(int i, int j, std::size_t nTriangles, PrimitiveIdentifier* pIds) {
	// For j outside the range [0, n - 1] return -1
	if (j < 0 || j > nTriangles - 1) {
		return -1;
	}
	const unsigned k0 = pIds[i].mortonCode;
	const unsigned k1 = pIds[j].mortonCode;
	// If the keys are identical, use the key identifiers i,j as a fallback. See s.4, p.2.
	if (k0 == k1) {
		return 31 - int(std::log2(i ^ j));
	}
	return 31 - int(std::log2(k0 ^ k1));
}

static int clz(int i, int j, std::size_t nTriangles, PrimitiveIdentifier* pIds) {
	// For j outside the range [0, n - 1] return -1
	if (j < 0 || j > nTriangles - 1) {
		return -1;
	}
	const unsigned k0 = pIds[i].mortonCode;
	const unsigned k1 = pIds[j].mortonCode;
	
	// If the keys are identical, use the key identifiers i,j as a fallback. See s.4, p.2.
	if (k0 == k1) {
		return (i + j) >> 1;
	}

	unsigned x = k0 ^ k1;

	static const char debruijn32[32] = {
		0, 31, 9, 30, 3, 8, 13, 29, 2, 5, 7, 21, 12, 24, 28, 19,
		1, 10, 4, 14, 6, 22, 25, 20, 11, 15, 23, 26, 16, 27, 17, 18
	};
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return debruijn32[x * 0x076be629 >> 27];
}

static unsigned int clz23(unsigned int k0, unsigned int k1) {
	unsigned int x = k0 ^ k1;
	static const char debruijn32[32] = {
		0, 31, 9, 30, 3, 8, 13, 29, 2, 5, 7, 21, 12, 24, 28, 19,
		1, 10, 4, 14, 6, 22, 25, 20, 11, 15, 23, 26, 16, 27, 17, 18
	};
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return debruijn32[x * 0x076be629 >> 27];
}

static int sign(int v) {
	return v < 0 ? -1 : ((v == 0) ? 0 : 1);
}

static int sign2(int v) {
	return v < 0 ? -1 : 1;
}

int findSplit(PrimitiveIdentifier* ids, int first, int last) {
	unsigned int key0 = ids[first].mortonCode;
	unsigned int key1 = ids[last].mortonCode;
	if (key0 == key1) {
		return (first + last) >> 1;
	}
	int commonPrefix = clz23(key0, key1);
	
	int split = first;
	int step = last - first;

	do {
		step = (step + 1) >> 1;
		int newSplit = split + step;
		if (newSplit < last) {
			unsigned int splitCode = ids[newSplit].mortonCode;
			int splitPrefix = clz23(key0, splitCode);
			if (splitPrefix > commonPrefix) {
				split = newSplit;
			}
		}
	} while (step > 1);
	
	return split;
}

void CPU_LBVH::generateHierarchy(const int i, const int nPrimitives, PrimitiveIdentifier* pIds) {
	int d = sign(clz(i, i + 1, nPrimitives, pIds) - clz(i, i - 1, nPrimitives, pIds));
	const int minPrefix = clz(i, i - d, nPrimitives, pIds);

	float lmax = 2;
	while (clz(i, i + lmax * d, nPrimitives, pIds) > minPrefix) {
		lmax *= 2;
	}

	float yy = 2.f;
	float l = 0;
	for (float t = lmax / yy; t >= 1.f; t = lmax / yy) {
		if (clz(i, i + (l + t) * d, nPrimitives, pIds) > minPrefix) {
			l += t;
		}
		yy *= 2.f;
	}
	
	int j = i + l * d;
	int dNode = clz(i, j, nPrimitives, pIds);

	float s = 0;
	float kk = 2.f;
	for (float t = ceil(l / kk); t > 1.f; t = ceil(l / kk)) {
		if (clz(i, i + (s + t) * d, nPrimitives, pIds) > dNode) {
			s += t;
		}
		kk *= 2.f;
	}
	if (clz(i, i + (s + 1.f) * d, nPrimitives, pIds) > dNode) {
			s += 1.f;
	}

	int split = i + s * d + std::min(d, 0);

	if (i == 0) {
		std::cout << "i: " << i << "\tj: " << j << "\td: " << d << "\tdNode: " << dNode << "\tl: " << l << "\ts: " << s << "\tsplit: " << split << '\n';
	}

	if (std::min(i, j) == split) {
		internalNodes[i].leftIdx = leafNodes[split].leafIdx;
		leafNodes[split].parentIdx = internalNodes[i].nodeIdx;
		// We mark this as a left leaf child
		internalNodes[i].childLeaf += 1;
	} else {
		internalNodes[i].leftIdx = internalNodes[split].nodeIdx;
		internalNodes[split].parentIdx = internalNodes[i].nodeIdx;
	}

	if (std::max(i, j) == split + 1) {
		internalNodes[i].rightIdx = leafNodes[split + 1].leafIdx;
		leafNodes[split + 1].parentIdx = internalNodes[i].nodeIdx;
		// We mark this as a right leaf child
		internalNodes[i].childLeaf += 2;
	} else {
		internalNodes[i].rightIdx = internalNodes[split + 1].nodeIdx;
		internalNodes[split + 1].parentIdx = internalNodes[i].nodeIdx;
	}
}

void CPU_LBVH::generateBV(AxisAlignedBoundingBox* aabbs, const int nTriangles) {
	// Bottom up reduction to create bounding volumes.
	std::vector<int> atomic_counters(nTriangles, false);
	
	for (int i = 0; i < nTriangles; ++i) {
		AxisAlignedBoundingBox aabb = aabbs[leafNodes[i].leafIdx];
		int parentIdx = leafNodes[i].parentIdx;
		if (parentIdx == -1) {
			std::cout << "Here: " << i << '\n';
		}
		while (parentIdx != -1) {
			if (atomic_counters[parentIdx] == false) {
				atomic_counters[parentIdx] = true;
				output_aabbs[parentIdx] = aabb;
				break;
			}
			output_aabbs[parentIdx] = combine(output_aabbs[parentIdx], aabb);
			aabb = output_aabbs[parentIdx];
			parentIdx = internalNodes[parentIdx].parentIdx;
		}
	}	
}

int CPU_LBVH::GetRootIndex() const {
	int nodeIdx = leafNodes[0].parentIdx;
	while (internalNodes[nodeIdx].parentIdx != -1) {
		nodeIdx = internalNodes[nodeIdx].parentIdx;
	}
	return nodeIdx;
}

bool CPU_LBVH::isValid() {
	bool result = true;
	int rootIdx = GetRootIndex();
	isValid_Rec(rootIdx, internalNodes[rootIdx].leftIdx, result);
	isValid_Rec(rootIdx, internalNodes[rootIdx].rightIdx, result);
	return result;
}

void CPU_LBVH::isValid_Rec(int nodeIdx, int childIdx, bool& result) {
	if (internalNodes[nodeIdx].childLeaf != 0) {
		return;
	}
	
	if (!result) {
		return;
	}

	auto trace_error_path = [this, nodeIdx, childIdx]()
	{
		std::cout << "Printing hierarchy bottom-up...\n";
		std::cout << "Hierarchy path: " << childIdx << " ";
		int parentIdx = nodeIdx;
		while (parentIdx != -1) {
			std::cout << nodeIdx << " ";
			parentIdx = internalNodes[parentIdx].parentIdx;
		}
		std::cout << parentIdx << " ";
	};

	if (output_aabbs[nodeIdx].isCollapsed()) {
		std::cout << "Bounding volume is collapsed\n";
		while (true) {
			int leftIdx = internalNodes[nodeIdx].leftIdx;
			bool isLeftInternal = !(internalNodes[nodeIdx].childLeaf & 1);
			bool isRightInternal = !(internalNodes[nodeIdx].childLeaf > 1);
			if (leftIdx != -1 && isLeftInternal) {
				if (output_aabbs[leftIdx].isCollapsed()) {
					std::cout << "[left] " << leftIdx << " collapsed\n";
				}
				nodeIdx = leftIdx;
				continue;
			} else if(leftIdx != -1 && !isLeftInternal) {
				std::cout << "[Left, Leaf] " << leafNodes[leftIdx].leafIdx << '\n';
				std::cout << "[Left, leaf, BV] " << aabbs[leafNodes[leftIdx].leafIdx] << '\n';
			}
			int rightIdx = internalNodes[nodeIdx].rightIdx;
			if (rightIdx != -1 && isRightInternal) {
				if (output_aabbs[rightIdx].isCollapsed()) {
					std::cout << "[right] " << rightIdx << " collapsed\n";
				}
				nodeIdx = rightIdx;
				continue;
			}
			break;
		}
		return;
	}

	if (output_aabbs[nodeIdx].containsEdgeInclusive(output_aabbs[childIdx])) {
		isValid_Rec(childIdx, internalNodes[childIdx].leftIdx, result);
		isValid_Rec(childIdx, internalNodes[childIdx].rightIdx, result);
	} else {
		result = false;
		std::cout << "\n\nError was triggered because parent doesn't contain child BV. See:\n";
		std::cout << "Parent(" << nodeIdx << "): " << output_aabbs[nodeIdx];
		std::cout << "Child(" << childIdx << "): " << output_aabbs[childIdx] << '\n';
		trace_error_path();
	}
}

void CPU_LBVH::isHierarchyValid_Rec(int parentIdx, int nodeIdx, bool& result) {
	if (parentIdx == -1) {
		return;
	}
}

}