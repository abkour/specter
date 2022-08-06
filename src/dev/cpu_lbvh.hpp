#pragma once
#include "cpu_lbvh_helpers.hpp"
#include "../model.hpp"
#include "../space_partition.hpp"

namespace specter {

struct InternalNode {
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

//tbb12.lib

// Reference implementation of the lbvh construction algorithm 
// developed by Tero Karras. 
// Paper: Maximizing Parallelism in the Construction of BVHs, Octrees, and k-d Trees
struct CPU_LBVH : public ISpacePartitioner {

	CPU_LBVH()
		: nTriangles(0)
	{}

	//
	// ISpacePartitioner Interface
	void build(std::shared_ptr<Model>& model) override;

	bool traverse(const Model* model, const Ray& ray, Intersection& intersection) const override;
	
	bool isValid();

protected:

	//
	// Implementation
	void traverse_Rec(const Model* model, const int nodeIdx, const Ray& ray, Intersection& its) const;

	int GetRootIndex() const;

	void generateHierarchy(const int i, const int nPrimitives, PrimitiveIdentifier* pids);


	void generateBV(AxisAlignedBoundingBox* aabbs, const int nTriangles);
	
	void isValid_Rec(int parentIdx, int nodeIdx, bool& result);

protected:

	std::size_t nTriangles;
	std::vector<LeafNode> leafNodes;
	std::vector<InternalNode> internalNodes;
	std::vector<AxisAlignedBoundingBox> output_aabbs;
};

}