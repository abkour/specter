#pragma once
#include "aabb.hpp"
#include "common_math.hpp"
#include "mesh.hpp"
#include "vec3.hpp"

#include <vector>

namespace specter {

struct Node {
	AxisAlignedBoundingBox bbox;

	Node** m_children = nullptr;
	uint32_t* indices = nullptr;
	uint32_t nIndices;
};

struct Octree {

	Octree();
	~Octree();

	void build(Mesh* mesh);
	bool traverse(const Mesh* mesh, const Ray& ray, float& u, float &v, float& t, uint32_t& index);
	bool intersectsAny(const Mesh* mesh, const Ray& ray);

private:
	
	void buildRec(Node* node, const vec3f* vertices, const FaceElement* faces, const uint32_t* trianglePositions, int depth = 0);
	void traverseRec(const Mesh* mesh, Node* node, const Ray& ray, float& u, float& v, float& t, uint32_t& index);
	void intersectsAnyRec(const Mesh* mesh, Node* node, const Ray& ray, bool& intersectionFound);

	void freeOctreeRec(Node* node);

private:

	Node* root;
	std::size_t maxDepth;

};

}