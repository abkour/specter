#pragma once
#include "light.hpp"

namespace specter {

class PointLight : public ILight {

public:

	PointLight() = default;

	// Initialize a light object in world space
	PointLight(const vec3f& pos, const vec3f& energy)
	{
		position = pos;
		this->energy = energy;
	}

	vec3f sample_light(const Accel& accel, const vec3f& point, const vec3f& normal) override;

protected:

	vec3f position;
	vec3f energy;
};

}