#include "coordinate_system.hpp"

namespace specter {

// Computes the coordinate system of a plane given its surface normal.
CoordinateSystem::CoordinateSystem(const vec3f& normal) {
	zaxis = normal;
	if (std::abs(normal.x) > std::abs(normal.y)) {
		yaxis = vec3f(normal.z, 0.f, -normal.x);
	} else {
		yaxis = vec3f(0.f, normal.z, -normal.y);
	}
	yaxis = normalize(yaxis);
	xaxis = cross(yaxis, zaxis);
}

// Transform vector from world space to local space
vec3f CoordinateSystem::toLocal(const vec3f& v) {
	return vec3f(dot(v, xaxis), dot(v, yaxis), dot(v, zaxis));
}

// Transform vector from world space to local space
vec3f CoordinateSystem::toWorld(const vec3f& v) {
	return v.x * xaxis + v.y * yaxis + v.z * zaxis;
}

}