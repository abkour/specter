#pragma once
#include "light.hpp"

namespace specter {

struct PointLight : public Light {

	PointLight() = default;

	// Initialize a light object in world space
	PointLight(const vec3f& pos, const vec3f& energy)
	{
		position = pos;
		this->energy = energy;
	}

	vec3f sample_Light(const vec3f& point, const vec3f& normal) const;
};

}