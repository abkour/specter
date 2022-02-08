#pragma once
#include "aabb.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace specter {

struct AxisAlignedBoundingBox {

	AxisAlignedBoundingBox();
	AxisAlignedBoundingBox(const vec3f& bmin, const vec3f& bmax);
	AxisAlignedBoundingBox(const AxisAlignedBoundingBox& other);
	AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& other);

	bool operator==(const AxisAlignedBoundingBox& other);
	bool operator!=(const AxisAlignedBoundingBox& other);

	bool contains(const vec3f& point);
	bool containsEdgeInclusive(const vec3f& point);
	bool overlaps(const AxisAlignedBoundingBox& other);
	bool rayIntersects(const Ray& ray);
	bool rayIntersects(const Ray& ray, float& near, float& far);

	vec3f min, max;
};

AxisAlignedBoundingBox::AxisAlignedBoundingBox() = default;

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const vec3f& bmin, const vec3f& bmax)
	: min(bmin)
	, max(bmax)
{}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const AxisAlignedBoundingBox& other) 
	: min(other.min)
	, max(other.max)
{}

AxisAlignedBoundingBox& AxisAlignedBoundingBox::operator=(const AxisAlignedBoundingBox& other) {
	min = other.min;
	max = other.max;
	return *this;
}

bool AxisAlignedBoundingBox::operator==(const AxisAlignedBoundingBox& other) {
	return min == other.min && max == other.max;
}

bool AxisAlignedBoundingBox::operator!=(const AxisAlignedBoundingBox& other) {
	return min != other.min || max != other.max;
}

bool AxisAlignedBoundingBox::contains(const vec3f& point) {
	return point < max && point > min;
}

bool AxisAlignedBoundingBox::containsEdgeInclusive(const vec3f& point) {
	return point <= max && point >= min;
}

bool AxisAlignedBoundingBox::overlaps(const AxisAlignedBoundingBox& other) {

}

vec3f center(const AxisAlignedBoundingBox& aabb) {
	return vec3f(aabb.min + (aabb.max - aabb.min) / 2);
}

float surfaceArea(const AxisAlignedBoundingBox& aabb) {
	const vec3f axesLengths = aabb.max - aabb.min;
	return axesLengths.x * axesLengths.y * 2 + axesLengths.x * axesLengths.z * 2 + axesLengths.y * axesLengths.z * 2;
}

float volume(const AxisAlignedBoundingBox& aabb) {
	return product(aabb.max - aabb.min);
}


}