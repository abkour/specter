#pragma once
#include "mesh.hpp"
#include "octree.hpp"
#include "ray.hpp"

namespace specter {

// Structure used to accelerate ray tracing.
// At the moment it is using an octree as it's internal accelerating structure.
// However, it is very easy to replace with something else like a k-d tree or BVH.
class Accel {

public:

	void addMesh(Mesh* newMesh);
	
	void build();

	bool traceRay(const Ray& ray, Intersection& intersection, bool shadowRay = false);

private:

	Mesh* mesh;

	Octree octree;
};

}