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

	~CPU_LBVH();

protected:

	void generateHierarchy(const int i, const int nPrimitives, PrimitiveIdentifier* pids);

protected:

	InternalNode* internalNodes;
	LeafNode* leafNodes;
	AxisAlignedBoundingBox* aabbs;
	AxisAlignedBoundingBox* output_aabbs;
};

}