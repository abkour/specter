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

	// Adds mesh to the accelerating structure. 
	// At the moment only one mesh is supported
	void addMesh(Mesh* newMesh);
	
	// Construct the underlying accelerating structure object.
	void build();

	// Trace ray using the accelerating structure. Returns true if the ray collides with mesh geometry.
	bool traceRay(const Ray& ray, Intersection& intersection, bool shadowRay = false);

private:

	Mesh* mesh;

	Octree octree;
};

}