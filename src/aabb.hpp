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
	bool rayIntersects(const Ray& ray) const;
	bool rayIntersects(const Ray& ray, float& near, float& far) const;
	bool rayIntersectsOptimistic(const Ray& ray, float& near, float& far) const;

	float surfaceArea() const;
	float volume() const;
	
	vec3f center() const;

	vec3f min, max;
};

}
