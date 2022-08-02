#pragma once
#include "octree.hpp"
#include "ray.hpp"

namespace specter {

// Structure used to accelerate ray tracing.
// At the moment it is using an octree as it's internal accelerating structure.
// However, it is very easy to replace with something else like a k-d tree or BVH.

// Generally, we could make each data structure (octree, k-d, bvh etc.) inherit from 
// a base interface and use a pointer to base in the accel data structure. This allows,
// the specification of the data structure at runtime, which can be good.
// Alternatively, we could use compile time flags to construct the Accel structure
// at compile time using the preprocessor. This is a little bit trickier and it requires
// a recompile if the user wants to change the internal accelerating structure. This solution,
// is not good, because the cost of accessing the vtable is not measurable.
class Accel {

public:

	// Adds model to the accelerating structure. 
	// At the moment only one mesh is supported
	void addModel(std::shared_ptr<Model>& model);
	
	// Construct the underlying accelerating structure object.
	void build();

	// Trace ray using the accelerating structure. Returns true if the ray collides with mesh geometry.
	bool traceRay(const Ray& ray, Intersection& intersection) const;

	// Trace shadow ray. Returns true if any intersection is found. Returns false otherwise.
	// The shadow ray functionality is currently deprecated, because I have switched the renderer.
	// In the past, I used to seperate the scene from the lights, which allows direct shadow queries.
	// Now, lights are simply treated as emitting surfaces, which are incorporated in the scene.
	// The motivatin for this change was in the difficulty of handling transperent objects.
	bool traceShadowRay(const Ray& ray) const;
	bool traceShadowRayTmax(const Ray& ray, const float t_max) const;

	// This should not really be used outside of debugging.
	decltype(auto) GetOctree() {
		return &octree;
	}

private:

	std::shared_ptr<Model> model;
	Octree octree;
};

}