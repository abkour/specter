#pragma once
#include "aabb.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace specter {

struct AxisAlignedBoundingBox {

	AxisAlignedBoundingBox() = default;
	AxisAlignedBoundingBox(const vec3f& bmin, const vec3f& bmax);
	AxisAlignedBoundingBox(const AxisAlignedBoundingBox& other);
	AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& other);

	bool operator==(const AxisAlignedBoundingBox& other) const;
	bool operator!=(const AxisAlignedBoundingBox& other) const;

	bool isValid() const;
	bool contains(const vec3f& point) const;
	bool containsEdgeInclusive(const vec3f& point) const;
	bool overlaps(const AxisAlignedBoundingBox& other) const;
	bool overlapsEdgeInclusive(const AxisAlignedBoundingBox& other) const;

	bool rayIntersect(const Ray& ray, float& nearT, float& farT) const;

	// Implements the slab method
	bool rayIntersects(const Ray& ray) const;

	// Implements the slab method
	// Near stores parameter t related to the entry intersection point.
	// Far stores the paramter t related to the exit intersection point.
	bool rayIntersects(const Ray& ray, float& near, float& far) const;

	// Implements the improved slab method.
	// Near stores parameter t related to the entry intersection point.
	// Far stores the paramter t related to the exit intersection point.
	// Additionally, it is assumed that ray.o is outside the bounding box. The algorithm is ill behaving otherwise.
	bool rayIntersectsOptimistic(const Ray& ray, float& near, float& far) const;

	float surfaceArea() const;
	float volume() const;
	
	vec3f center() const;

	vec3f min, max;
};

std::ostream& operator<<(std::ostream& os, const AxisAlignedBoundingBox& aabb);

}
