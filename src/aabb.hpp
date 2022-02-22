#pragma once
#include "ray.hpp"
#include "vec3.hpp"

namespace specter {

struct AxisAlignedBoundingBox {

	// Constructs a collapsed AABB
	AxisAlignedBoundingBox();
	// Constructs a bounding box spanning [bmin, bmax]
	AxisAlignedBoundingBox(const vec3f& bmin, const vec3f& bmax);
	// Constructs a copy of bounding box \other
	AxisAlignedBoundingBox(const AxisAlignedBoundingBox& other);
	// Copy assign \other to *this
	AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox& other);

	// Compares if two bounding boxes occupy the same space
	bool operator==(const AxisAlignedBoundingBox& other) const;
	// Compares if two bounding boxes occupy different spaces
	bool operator!=(const AxisAlignedBoundingBox& other) const;

	// Returns the validity of the bounding box. Returns false if min > max. The bounding box is ill-defined.
	bool isValid() const;
	// Returns true if the bbox has volume 0
	bool isCollapsed() const;
	// Returns true if point is confined within (min, max)
	bool contains(const vec3f& point) const;
	// Returns true if point is confined within [min, max]
	bool containsEdgeInclusive(const vec3f& point) const;
	// Returns true if two aabbs overlap in space.
	bool overlaps(const AxisAlignedBoundingBox& other) const;
	// Additionally, returns true if two edges/faces overlap
	bool overlapsEdgeInclusive(const AxisAlignedBoundingBox& other) const;
	
	// Returns true if the ray collides with the bounding box along it's travelling direction
	bool rayIntersect(const Ray& ray, float& nearT, float& farT) const;

	// Return the surface area of the bounding box
	float surfaceArea() const;
	// Return the volume of the bounding box
	float volume() const;
	// Return the center of the bounding box
	vec3f center() const;

	vec3f min, max;
};

std::ostream& operator<<(std::ostream& os, const AxisAlignedBoundingBox& aabb);

}
