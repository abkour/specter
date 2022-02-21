#pragma once
#include "vec3.hpp"

namespace specter {

struct CoordinateSystem {

	CoordinateSystem() = default;

	// Computes the coordinate system of a plane given its surface normal.
	CoordinateSystem(const vec3f& normal);

	// Transform vector from world space to local space
	vec3f toLocal(const vec3f& v);

	// Transform vector from local space to world space
	vec3f toWorld(const vec3f& v);

	// Normal, normal tangent and normal bitangent vectors
	vec3f n, nt, nb;
};

}