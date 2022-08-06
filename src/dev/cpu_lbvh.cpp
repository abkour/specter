#include "cpu_lbvh.hpp"

namespace specter {


// Implementation
static void assignObjectID(const int idx, InternalNode* internalNodes, LeafNode* leafNodes, PrimitiveIdentifier* sortedObjects) {
	internalNodes[idx].nodeIdx = idx;
	internalNodes[idx].leftIdx = -1;
	internalNodes[idx].rightIdx = -1;
	internalNodes[idx].parentIdx = -1;
	internalNodes[idx].childLeaf = 0;
	leafNodes[idx].parentIdx = -1;
	leafNodes[idx].leafIdx = sortedObjects[idx].objectId;
}

void CPU_LBVH::build(std::shared_ptr<Model>& model) {
	nTriangles = model->GetFaceCount() / 3;
	PrimitiveIdentifier* pIds = new PrimitiveIdentifier[nTriangles];
	internalNodes = new InternalNode[nTriangles - 1];
	leafNodes = new LeafNode[nTriangles];
	aabbs = new AxisAlignedBoundingBox[nTriangles];
	output_aabbs = new AxisAlignedBoundingBox[nTriangles - 1];

	const AxisAlignedBoundingBox scene_box = model->computeBoundingBox();

	for (int i = 0; i < nTriangles; ++i) {
		aabbs[i] = constructAABBFromPaddedTriangle(
			model->GetVertex(model->GetFace(i * 3).p),
			model->GetVertex(model->GetFace(i * 3 + 1).p),
			model->GetVertex(model->GetFace(i * 3 + 2).p));
	}

	for (int i = 0; i < nTriangles; ++i) {
		computeMortonCode(pIds, aabbs, i, scene_box.min, scene_box.max);
	}

	// Sort primitives according to their morton codes
	radixsort(pIds, nTriangles);

	for (int i = 0; i < nTriangles; ++i) {
		assignObjectID(i, internalNodes, leafNodes, pIds);
	}

	for (int i = 0; i < nTriangles - 1; ++i) {
		generateHierarchy(i, nTriangles, pIds);
	}

	generateBV(nTriangles);

	delete[] pIds;
	delete[] aabbs;
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

static int clz(int i, int j, std::size_t nTriangles, PrimitiveIdentifier* pIds) {
	if (j < 0 || j > nTriangles - 1) return -1;
	const unsigned k0 = pIds[i].mortonCode;
	const unsigned k1 = pIds[j].mortonCode;
	if (k0 == k1) {
		return 31;	// Avoid log2(0)
	}
	return 31 - int(log2(float(k0 ^ k1)));
}

static int sign(float v) {
	return ((v < 0.f) ? -1.f : ((v > 0.f) ? 1.f : 0.f));
}

CPU_LBVH::~CPU_LBVH() {
#ifdef _DEBUG
	std::cout << "Deleting Octree!\n";
#endif
	if (output_aabbs != nullptr) {
		delete[] output_aabbs;
		output_aabbs = nullptr;
	}
	if (internalNodes != nullptr) {
		delete[] internalNodes;
		internalNodes = nullptr;
	}
	if (leafNodes != nullptr) {
		delete[] leafNodes;
		leafNodes = nullptr;
	}
}

void CPU_LBVH::generateHierarchy(const int i, const int nPrimitives, PrimitiveIdentifier* pIds) {
	int d = sign(clz(i, i + 1, nPrimitives, pIds) - clz(i, i - 1, nPrimitives, pIds));
	const int minPrefix = clz(i, i - d, nPrimitives, pIds);

	int lmax = 2;
	while (clz(i, i + lmax * d, nPrimitives, pIds) > minPrefix) {
		lmax *= 2;
	}

	int l = 0;
	for (int t = lmax / 2; t >= 1; t = t / 2) {
		if (clz(i, i + (l + t) * d, nPrimitives, pIds) > minPrefix) {
			l += t;
		}
	}

	const int j = i + l * d;
	int dNode = clz(i, j, nPrimitives, pIds);

	int s = 0;
	int t = (int)ceil((float)l / 2.f);
	while (t > 1) {
		if (clz(i, i + (s + t) * d, nPrimitives, pIds) > dNode) {
			s += t;
		}
		t = (int)ceil((float)t / 2.f);
	}
	// The case for t = 1
	if (clz(i, i + (s + 1) * d, nPrimitives, pIds) > dNode) s += 1;

	const int split = i + s * d + std::min(d, 0);

	if (std::min(i, j) == split) {
		internalNodes[i].leftIdx = leafNodes[split].leafIdx;
		leafNodes[split].parentIdx = internalNodes[i].nodeIdx;
		internalNodes[i].childLeaf += 1;
	} else {
		internalNodes[i].leftIdx = internalNodes[split].nodeIdx;
		internalNodes[split].parentIdx = internalNodes[i].nodeIdx;
	}

	if (std::max(i, j) == split + 1) {
		internalNodes[i].rightIdx = leafNodes[split + 1].leafIdx;
		leafNodes[split + 1].parentIdx = internalNodes[i].nodeIdx;
		internalNodes[i].childLeaf += 2;
	} else {
		internalNodes[i].rightIdx = internalNodes[split + 1].nodeIdx;
		internalNodes[split + 1].parentIdx = internalNodes[i].nodeIdx;
	}
}

void CPU_LBVH::generateBV(const int nTriangles) {
	// Bottom up reduction to create bounding volumes.
	const int nAtomicCounters = nTriangles;
	const int nLeafNodes = nTriangles;
	bool* atomic_counters = new bool[nAtomicCounters](false);

	int* tmp_parentIdx = new int[nLeafNodes];
	for (int i = 0; i < nLeafNodes; ++i) {
		tmp_parentIdx[i] = leafNodes[i].parentIdx;
	}

	for (int i = 0; i < nLeafNodes; ++i) {
		AxisAlignedBoundingBox aabb = aabbs[leafNodes[i].leafIdx];
		int parentIdx = tmp_parentIdx[i];
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

	delete[] tmp_parentIdx;
	delete[] atomic_counters;
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
	
	if (output_aabbs[nodeIdx].containsEdgeInclusive(output_aabbs[childIdx])) {
		isValid_Rec(childIdx, internalNodes[childIdx].leftIdx, result);
		isValid_Rec(childIdx, internalNodes[childIdx].rightIdx, result);
	} else {
		result = false;
#ifdef _DEBUG
		std::cout << "Parent(" << nodeIdx << "): " << output_aabbs[nodeIdx];
		std::cout << "Child(" << childIdx << "): " << output_aabbs[childIdx] << '\n';
#endif
	}
}

}