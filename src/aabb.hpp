#pragma once
#include "vec3.hpp"
#include "ray.hpp"

namespace specter {

struct AxisAlignedBoundingBox {

	AxisAlignedBoundingBox();
	AxisAlignedBoundingBox(const vec3f& bmin, const vec3f& bmax);
	AxisAlignedBoundingBox(const AxisAlignedBoundingBox& other);
	AxisAlignedBoundingBox(AxisAlignedBoundingBox&& other);
	AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& other);

	bool operator==(const AxisAlignedBoundingBox& other);
	bool operator!=(const AxisAlignedBoundingBox& other);

	bool contains(const vec3f& point);
	
	bool overlaps(const AxisAlignedBoundingBox& other);

	bool rayIntersects(const Ray& ray);
	bool rayIntersects(const Ray& ray, float& near, float& far);

	vec3f min, max;
};

vec3f center(const AxisAlignedBoundingBox& aabb);

float surfaceArea(const AxisAlignedBoundingBox& aabb);

float volume(const AxisAlignedBoundingBox& aabb);


}