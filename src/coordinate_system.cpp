#include "coordinate_system.hpp"

namespace specter {

// Computes the coordinate system of a plane given its surface normal.
CoordinateSystem::CoordinateSystem(const vec3f& normal) {
	zaxis = normal;
	if (std::abs(normal.x) > std::abs(normal.y)) {
		xaxis = vec3f(normal.z, 0.f, -normal.x);
	} else {
		xaxis = vec3f(0.f, normal.z, -normal.y);
	}
	xaxis = normalize(xaxis);
	yaxis = cross(xaxis, zaxis);
}

}