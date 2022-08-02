#pragma once
#include "cpu_lbvh_helpers.hpp"
#include "../model.hpp"

namespace specter {

struct InternalNode {
	InternalNode() 
		: nodeIdx(0), leftIdx(0), rightIdx(0), parentIdx(0), childLeaf(0)
	{}

	int nodeIdx;
	int leftIdx;
	int rightIdx;
	int parentIdx;
	unsigned childLeaf;
};

struct LeafNode {
	int parentIdx;
	int leafIdx;
};


// Reference implementation of the lbvh construction algorithm 
// developed by Tero Karras. 
// Paper: Maximizing Parallelism in the Construction of BVHs, Octrees, and k-d Trees
struct CPU_LBVH {

	CPU_LBVH() = default;
	
	void prepass(Model& model);

	AxisAlignedBoundingBox* GetBoundingVolumes() {
		return output_aabbs;
	}

	~CPU_LBVH();

protected:

	void generateHierarchy(const int i, const int nPrimitives, PrimitiveIdentifier* pids);

	void generateBV(const int nTriangles);
	
	// On the GPU, this action is relatively easy to implement using atomic counters as outlined
	// by Tero Karras. However, on the CPU you can also consider performing a DFS for O(1) 
	// memory complexity. In the CPU implementation, I will not be using atomic counters. Instead,
	// I will treat the layers of a tree as if they were in contigious memory,
	void generateBVBottomUpRecursively(const int nodeIdx, const int parentIdx);

protected:

	InternalNode* internalNodes;
	LeafNode* leafNodes;
	AxisAlignedBoundingBox* aabbs;
	AxisAlignedBoundingBox* output_aabbs;
};

}