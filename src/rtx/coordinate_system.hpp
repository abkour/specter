#pragma once
#include "../math/vec3.hpp"

namespace specter {

// This class represents a coordinate system with orthonormal bases.
// Functionally, this class allows rebasing a vector from an implicit 
// coordinate system to the one specified by this one, and back again.
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