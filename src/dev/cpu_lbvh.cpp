#include "cpu_lbvh.hpp"

namespace specter {

static void assignObjectID(const int idx, InternalNode* internalNodes, LeafNode* leafNodes, PrimitiveIdentifier* sortedObjects) {
	internalNodes[idx].nodeIdx = idx;
	internalNodes[idx].leftIdx = -1;
	internalNodes[idx].rightIdx = -1;
	internalNodes[idx].parentIdx = -1;
	internalNodes[idx].childLeaf = 0;
	leafNodes[idx].parentIdx = -1;
	leafNodes[idx].leafIdx = sortedObjects[idx].objectId;
}

static int clz(int i, int j, std::size_t nTriangles, PrimitiveIdentifier* pIds) {
	if (i < 0 || j > nTriangles - 1) return -1;
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
	delete[] output_aabbs;
	delete[] internalNodes;
	delete[] leafNodes;
}

void CPU_LBVH::prepass(Model& model) {
	const std::size_t nTriangles = model.GetFaceCount() * 3;
	PrimitiveIdentifier* pIds = new PrimitiveIdentifier[nTriangles];
	int* atomic_counters = new int[nTriangles - 1]();
	internalNodes = new InternalNode[nTriangles - 1];
	leafNodes = new LeafNode[nTriangles];
	aabbs = new AxisAlignedBoundingBox[nTriangles];	
	output_aabbs = new AxisAlignedBoundingBox[nTriangles - 1];

	const AxisAlignedBoundingBox scene_box = model.computeBoundingBox();

	for (int i = 0; i < nTriangles; ++i) {
		aabbs[i] = constructAABBFromPaddedTriangle(
			model.GetVertex(i * 3),
			model.GetVertex(i * 3 + 1),
			model.GetVertex(i * 3 + 2));
	}

	for (int i = 0; i < nTriangles; ++i) {
		computeMortonCode(pIds, aabbs, i, scene_box.min, scene_box.max);
	}

	// Sort primitives according to their morton codes
	radixsort(pIds, nTriangles);

	assignObjectID(nTriangles - 1, internalNodes, leafNodes, pIds);
	for (int i = 0; i < nTriangles - 1; ++i) {
		assignObjectID(i, internalNodes, leafNodes, pIds);
	}

	for (int i = 0; i < nTriangles - 1; ++i) {
		generateHierarchy(i, nTriangles, pIds);
	}

	delete[] atomic_counters;
	delete[] pIds;
	delete[] aabbs;
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

}