#pragma once 
#include "vec3.hpp"

namespace specter {

struct Light {
	
	// Initialize a light object in world space
	Light(const vec3f& pos, const vec3f& energy)
		: position(pos)
		, energy(energy)
	{}

	vec3f energy;
	vec3f position;
};

}