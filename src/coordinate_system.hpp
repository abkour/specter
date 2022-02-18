#pragma once
#include "vec3.hpp"

namespace specter {

struct CoordinateSystem {

	CoordinateSystem() = default;

	// Computes the coordinate system of a plane given its surface normal.
	CoordinateSystem(const vec3f& normal);

	// Transform vector from world space to local space
	vec3f toLocal(const vec3f& v);

	vec3f xaxis, yaxis, zaxis;
};

}