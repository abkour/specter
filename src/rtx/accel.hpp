#pragma once
#include "octree.hpp"
#include "ray.hpp"
#include "space_partition.hpp"

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

	bool isValid();

	// Trace ray using the accelerating structure. Returns true if the ray collides with mesh geometry.
	bool traceRay(const Ray& ray, Intersection& intersection) const;

private:

	std::shared_ptr<Model> model;
	std::shared_ptr<ISpacePartitioner> accel_struct;
};

}