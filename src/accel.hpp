#pragma once
#include "octree.hpp"
#include "ray.hpp"

namespace specter {

// Structure used to accelerate ray tracing.
// At the moment it is using an octree as it's internal accelerating structure.
// However, it is very easy to replace with something else like a k-d tree or BVH.
class Accel {

public:

	void dbg_print();

	// Adds model to the accelerating structure. 
	// At the moment only one mesh is supported
	void addModel(std::shared_ptr<Model>& model);
	
	// Construct the underlying accelerating structure object.
	void build();

	// Trace ray using the accelerating structure. Returns true if the ray collides with mesh geometry.
	bool traceRay(const Ray& ray, Intersection& intersection) const;

	// Trace shadow ray. Returns true if any intersection is found. Returns false otherwise.
	bool traceShadowRay(const Ray& ray) const;

	bool traceShadowRayTmax(const Ray& ray, const float t_max) const;

private:

	std::shared_ptr<Model> model;
	Octree octree;
};

}