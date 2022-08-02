#pragma once 
#include "accel.hpp"
#include "vec3.hpp"

namespace specter {

// Deprecated. Used in a previous version of this program, but no longer needed.
// Might use this in the future still.
class ILight {
	
public:

	virtual ~ILight() = 0 {}

	virtual vec3f sample_light(const Accel& accel, const vec3f& point, const vec3f& normal) = 0;
};

}