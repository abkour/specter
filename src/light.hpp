#pragma once 
#include "vec3.hpp"

namespace specter {

class Light {
	
public:

	virtual ~Light() = 0 {}

	virtual vec3f sample_Light(const vec3f& point, const vec3f& normal) const = 0;

	vec3f energy;
	vec3f position;
};

}