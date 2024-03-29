#pragma once
#include "aabb.hpp"
#include "common_math.hpp"
#include "model.hpp"
#include "vec3.hpp"

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

#include <vector>

namespace specter {

struct Octree {
	
	using layerIndex = int;
	
	// RAII -> set root to nullptr
	Octree();
	// RAII -> delete octree
	~Octree();

	void build(std::shared_ptr<Model>& model);

	// traverse the octree. Returns true if the ray intersects geometry in the mesh. 
	// Additionally, it is guaranteed that the intersection point is the closest intersection point to the ray origin
	bool traverse(const Model* model, const Ray& ray, Intersection& intersection) const;
	
	// Traverse the octree. Returns true if the ray intersects any geometry in the mesh.
	bool traverseAny(const Model* model, const Ray& ray) const;

	// Traverse the octree. Returns true if ray intersects geometry for t < t_max.
	// Returns false otherwise.
	bool traverseAnyTmax(const Model* mesh, const Ray& ray, const float t_max) const;

	std::pair<layerIndex, int> GetMaxBreadth() const;
	unsigned GetMaxDepth() const;

	void printNodesPerLayer() const;

private:

	// Represents a single node in the octree data structure
	// If the node is a leaf node, indices pointer should not be nullptr
	struct Node {
		
		void CreateValidNode(const sAABB* superRegion, int superRegionIdx, const std::size_t TriangleSize);

		void CreateInvalidNode() {
			this->nTriangles = 0;
		}

		bool IsValid() const {
			return nTriangles != 0;
		}
		
		bool IsInterior() const {
			return subboxes != nullptr && m_children != nullptr;
		}
		
		bool IsLeaf() const { 
			return tri_indices != nullptr;
		}

		bool HasChildren() const {
			return m_children != nullptr;
		}
		
		sAABB* subboxes = nullptr;
		// Pointer to the children nodes. This is nullptr for leaf nodes.
		Node* m_children = nullptr;
		// Pointer to the indices. This is nullptr for interior nodes
		uint32_t* tri_indices = nullptr;
		// Specifies the number of indices in the indices array, if it exists, otherwise specifies number of triangle indices
		// used during octree constructuin
		uint32_t nTriangles = 0;
	};

private:
	
	void MaxBreadthTraversal(Node* node, unsigned layer, unsigned* breadth) const;
	unsigned MaxDepthTraversal(Node* node, unsigned maxDepth) const;

private:

	// Build the octree recursively. This is initially called by the public function build()
	void buildRec(Node* node, const vec3f* vertices, const FaceElement* faces, const uint32_t* trianglePositions, int depth = 0);

	// Traverse the octree recursively. This is initially called by the public function traverse()
	void traverseRec(const Model* model, Node* node, const Ray& ray, Intersection& intersection, bool multipleBoxesHit = false) const;
	
	void traverseEdge(const Model* model, Node* node, const Ray& ray, Intersection& its);

	void computeTriangleIntersections(const Model* model, Node* node, const Ray& ray, Intersection& its) const;

	// Traverse the octree recursively. This is initially called by the public function traverseAny()
	void traverseAnyRec(const Model* model, Node* node, const Ray& ray, bool& intersectionFound) const;

	// Traverse the octree recursively. This is initially called by the public function traverseAnyTmax()
	void traverseAnyTmaxRec(const Model* model, Node* node, const Ray& ray, const float t_max, bool& intersectionFound) const;

	// Free the octree recursively. This is called by the destructor.
	void freeOctreeRec(Node* node);

private:

	Node* root;	// Represents the root of the octree
	std::size_t maxDepth;	// Specifies the maximum depth the octree can have. This parameter is used to avoid infinite depth octrees in pathological cases.

};

}