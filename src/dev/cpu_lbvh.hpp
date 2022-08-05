#pragma once
#include "cpu_lbvh_helpers.hpp"
#include "../model.hpp"
#include "../space_partition.hpp"

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
struct CPU_LBVH : public ISpacePartitioner {

	CPU_LBVH()
		: internalNodes(nullptr)
		, leafNodes(nullptr)
		, aabbs(nullptr)
		, output_aabbs(nullptr)
		, nTriangles(0)
	{}
	
	//
	// ISpacePartitioner Interface
	void build(std::shared_ptr<Model>& model) override;

	bool traverse(const Model* model, const Ray& ray, Intersection& intersection) const override;
	
	bool isValid();

	~CPU_LBVH();

protected:

	//
	// Implementation
	void traverse_Rec(const Model* model, const int nodeIdx, const Ray& ray, Intersection& its) const;

	int GetRootIndex() const;

	void generateHierarchy(const int i, const int nPrimitives, PrimitiveIdentifier* pids);

	void generateBV(const int nTriangles);
	
	void isValid_Rec(int parentIdx, int nodeIdx, bool& result);

protected:

	std::size_t nTriangles;
	InternalNode* internalNodes;
	LeafNode* leafNodes;
	AxisAlignedBoundingBox* aabbs;
	AxisAlignedBoundingBox* output_aabbs;
};

}