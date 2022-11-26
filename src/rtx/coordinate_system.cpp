#include "coordinate_system.hpp"

namespace specter {

// Computes the coordinate system of a plane given its surface normal.
CoordinateSystem::CoordinateSystem(const vec3f& normal) {
	n = normal;
	if (std::abs(normal.x) > std::abs(normal.y)) {
		nt = vec3f(normal.z, 0.f, -normal.x);
	} else {
		nt = vec3f(0.f, -normal.z, normal.y);
	}
	nb = cross(n, nt);
}

// Transform vector from world space to local space
vec3f CoordinateSystem::toLocal(const vec3f& v) {
	return vec3f(	v.x * nb.x + v.y * n.x + v.z * nt.x,
					v.x * nb.y + v.y * n.y + v.z * nt.y,
					v.x * nb.z + v.y * n.z + v.z * nt.z);
}

// Transform vector from local space to world space
vec3f CoordinateSystem::toWorld(const vec3f& v) {
	return v.x * nt + v.y * n + v.z * nb;
}

}