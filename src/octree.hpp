#pragma once
#include "aabb.hpp"
#include "common_math.hpp"
#include "mesh.hpp"
#include "vec3.hpp"

#include <vector>

namespace specter {

// Represents a single node in the octree data structure
// If the node is a leaf node, indices pointer should not be nullptr
struct Node {
	AxisAlignedBoundingBox bbox;	// Bounding box of the subdivided space

	Node** m_children = nullptr;	// Pointer to the children nodes
	uint32_t* indices = nullptr;	// Pointer to the indices. This is nullptr for interior nodes
	// Specifies the number of indices in the indices array, if it exists, otherwise specifies number of triangle indices
	// used during octree constructuin
	uint32_t nIndices;
};

struct Octree {
	
	// RAII -> set root to nullptr
	Octree();
	// RAII -> delete octree
	~Octree();

	// build the octree
	void build(Mesh* mesh);
	
	// traverse the octree. Returns true if the ray intersects geometry in the mesh. 
	// Additionally, it is guaranteed that the intersection point is the closest intersection point to the ray origin
	bool traverse(const Mesh* mesh, const Ray& ray, Intersection& intersection);
	
	// Traverse the octree. Returns true if the ray intersects any geometry in the mesh.
	bool traverseAny(const Mesh* mesh, const Ray& ray);

private:
	
	// Build the octree recursively. This is initially called by the public function build()
	void buildRec(Node* node, const vec3f* vertices, const FaceElement* faces, const uint32_t* trianglePositions, int depth = 0);
	
	// Traverse the octree recursively. This is initially called by the public function traverse()
	void traverseRec(const Mesh* mesh, Node* node, const Ray& ray, Intersection& intersection, bool multipleBoxesHit = false);
	
	void traverseAnyRec(const Mesh* mesh, Node* node, const Ray& ray, bool& intersectionFound);

	void freeOctreeRec(Node* node);

private:

	Node* root;
	std::size_t maxDepth;

};

}