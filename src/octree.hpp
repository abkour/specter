#pragma once
#include "aabb.hpp"
#include "common_math.hpp"
#include "dev/model.hpp"
#include "vec3.hpp"

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

#include <vector>

namespace specter {

struct Octree {
	
	// RAII -> set root to nullptr
	Octree();
	// RAII -> delete octree
	~Octree();

	static void dbg_print();

	void build(std::shared_ptr<Model>& model);

	// traverse the octree. Returns true if the ray intersects geometry in the mesh. 
	// Additionally, it is guaranteed that the intersection point is the closest intersection point to the ray origin
	bool traverse(const Model* model, const Ray& ray, Intersection& intersection) const;
	
	// Traverse the octree. Returns true if the ray intersects any geometry in the mesh.
	bool traverseAny(const Model* model, const Ray& ray) const;

	// Traverse the octree. Returns true if ray intersects geometry for t < t_max.
	// Returns false otherwise.
	bool traverseAnyTmax(const Model* mesh, const Ray& ray, const float t_max) const;

private:

	// Represents a single node in the octree data structure
	// If the node is a leaf node, indices pointer should not be nullptr
	struct Node {
		AxisAlignedBoundingBox bbox;	// Bounding box of the subdivided space

		Node** m_children = nullptr;	// Pointer to the children nodes
		uint32_t* tri_indices = nullptr;	// Pointer to the indices. This is nullptr for interior nodes
		// Specifies the number of indices in the indices array, if it exists, otherwise specifies number of triangle indices
		// used during octree constructuin
		uint32_t nTriangles;
	};

private:
	
	// Build the octree recursively. This is initially called by the public function build()
	void buildRec(Node* node, const vec3f* vertices, const FaceElement* faces, const uint32_t* trianglePositions, int depth = 0);

	// Traverse the octree recursively. This is initially called by the public function traverse()
	void traverseRec(const Model* model, Node* node, const Ray& ray, Intersection& intersection, bool multipleBoxesHit = false) const;
	
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