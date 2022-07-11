#pragma once 
#include "accel.hpp"
#include "vec3.hpp"

namespace specter {

class Light {
	
public:

	virtual ~Light() = 0 {}

	virtual vec3f sample_light(const Accel& accel, const vec3f& point, const vec3f& normal) = 0;
};

}