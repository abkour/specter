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

void buildOctree(Node* node, const vec3f* const vertices, const FaceElement* const faces, const uint32_t* trianglePositions, int depth = 0);
void freeOctree(Node* node);

float rayTraversal(const Mesh* mesh, Node* node, const Ray& ray, uint32_t& index);
void reinit();
void rayTraversal2(const Mesh* mesh, Node* node, const Ray& ray, float& u, float& v, float& t, uint32_t& index);

float rayTraversal_sorted(const Mesh* mesh, Node* node, const Ray& ray, uint32_t& index);

void printStatistics();
void printTraversal(vec2u screenResolution);

}