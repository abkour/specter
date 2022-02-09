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

bool AxisAlignedBoundingBox::operator==(const AxisAlignedBoundingBox& other) const {
	return min == other.min && max == other.max;
}

bool AxisAlignedBoundingBox::operator!=(const AxisAlignedBoundingBox& other) const {
	return min != other.min || max != other.max;
}

bool AxisAlignedBoundingBox::isValid() const {
	return min < max;
}

bool AxisAlignedBoundingBox::contains(const vec3f& point) const {
	return point < max && point > min;
}

bool AxisAlignedBoundingBox::containsEdgeInclusive(const vec3f& point) const {
	return point <= max && point >= min;
}

bool AxisAlignedBoundingBox::overlaps(const AxisAlignedBoundingBox& other) const {
	return min < other.max&& max > other.min;
}

bool AxisAlignedBoundingBox::overlapsEdgeInclusive(const AxisAlignedBoundingBox& other) const {
	return min <= other.max&& max >= other.min;
}

// Implements the improved slab, credit to Tavian Barnes. Modified to the 3D case.
bool AxisAlignedBoundingBox::rayIntersects(const Ray& ray) const {
	double tx1 = (min.x - ray.o.x) * ray.invd.x;
	double tx2 = (max.x - ray.o.x) * ray.invd.x;

	double tmin = std::min(tx1, tx2);
	double tmax = std::max(tx1, tx2);

	double ty1 = (min.y - ray.o.y) * ray.invd.y;
	double ty2 = (max.y - ray.o.y) * ray.invd.y;

	tmin = std::min(ty1, ty2);
	tmax = std::max(ty1, ty2);

	double tz1 = (min.z - ray.o.z) * ray.invd.z;
	double tz2 = (max.z - ray.o.z) * ray.invd.z;

	tmin = std::min(tz1, tz2);
	tmax = std::max(tz1, tz2);

	return tmin <= tmax;
}

// Implements the improved slab, credit to Tavian Barnes. Modified to the 3D case.
// Near stores parameter t related to the entry intersection point.
// Far stores the paramter t related to the exit intersection point.
bool AxisAlignedBoundingBox::rayIntersects(const Ray& ray, float& near, float& far) const {
	double tx1 = (min.x - ray.o.x) * ray.invd.x;
	double tx2 = (max.x - ray.o.x) * ray.invd.x;

	double tmin = std::min(tx1, tx2);
	double tmax = std::max(tx1, tx2);

	double ty1 = (min.y - ray.o.y) * ray.invd.y;
	double ty2 = (max.y - ray.o.y) * ray.invd.y;

	tmin = std::min(ty1, ty2);
	tmax = std::max(ty1, ty2);

	double tz1 = (min.z - ray.o.z) * ray.invd.z;
	double tz2 = (max.z - ray.o.z) * ray.invd.z;

	tmin = std::min(tz1, tz2);
	tmax = std::max(tz1, tz2);

	// In case tmin < 0, near has to be set to tmax.
	if (tmin < 0) {
		near = tmax;
		far = tmin;
	} else {
		near = tmin;
		far = tmax;
	}
	
	return tmin <= tmax;
}

// Implements the improved slab, credit to Tavian Barnes. Modified to the 3D case.
// Near stores parameter t related to the entry intersection point.
// Far stores the paramter t related to the exit intersection point.
// Additionally, it is assumed that ray.o is outside the bounding box. The algorithm is ill behaving otherwise.
bool AxisAlignedBoundingBox::rayIntersectsOptimistic(const Ray& ray, float& near, float& far) const {
	double tx1 = (min.x - ray.o.x) * ray.invd.x;
	double tx2 = (max.x - ray.o.x) * ray.invd.x;

	double tmin = std::min(tx1, tx2);
	double tmax = std::max(tx1, tx2);

	double ty1 = (min.y - ray.o.y) * ray.invd.y;
	double ty2 = (max.y - ray.o.y) * ray.invd.y;

	tmin = std::min(ty1, ty2);
	tmax = std::max(ty1, ty2);

	double tz1 = (min.z - ray.o.z) * ray.invd.z;
	double tz2 = (max.z - ray.o.z) * ray.invd.z;

	tmin = std::min(tz1, tz2);
	tmax = std::max(tz1, tz2);

	// Case tmin < 0 is not checked. Application needs to guaranteed that ray.origin is never inside the aabb.
	near = tmin;
	far = tmax;

	return tmin <= tmax;
}


vec3f AxisAlignedBoundingBox::center() const {
	return vec3f(min + (max - min) / 2);
}

float AxisAlignedBoundingBox::surfaceArea() const {
	const vec3f axesLengths = max - min;
	return axesLengths.x * axesLengths.y * 2 + axesLengths.x * axesLengths.z * 2 + axesLengths.y * axesLengths.z * 2;
}

float AxisAlignedBoundingBox::volume() const {
	return product(max - min);
}

}

/*

A overlaps B <-> B overlaps A:

	1. Point of A in B
	2. Point of B in A

	1) Point of A in B
	Approach a: Compute points of aabb and check for containment in B
	Approach b: 

	2) Same as 1. 

*/