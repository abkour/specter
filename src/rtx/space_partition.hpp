#pragma once
#include "../model.hpp"

namespace specter {

class ISpacePartitioner {

public:

	virtual void build(std::shared_ptr<Model>& model) = 0;

	// traverse the octree. Returns true if the ray intersects geometry in the mesh. 
	// Additionally, it is guaranteed that the intersection point is the closest intersection point to the ray origin
	virtual bool traverse(const Model* model, const Ray& ray, Intersection& intersection) const = 0;

	virtual bool isValid() = 0;
};

}