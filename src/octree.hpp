#pragma once
#include "aabb.hpp"
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

void buildOctree(Node* node, const vec3f* const vertices, const vec3u* const faces, const uint32_t* trianglePositions, int depth = 0);
void freeOctree(Node* node);

void rayTraversal(const Mesh* mesh, Node* node, const Ray& ray, float& u, float& v, float& mint, uint32_t& index);

void printOctreeNumbers();

}