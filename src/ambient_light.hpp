#pragma once
#include "coordinate_system.hpp"
#include "light.hpp"
#include "sampler.hpp"

namespace specter {

struct AmbientLight : public Light {

	AmbientLight() = default;

	vec3f sample_light(const vec3f& point, const vec3f& normal);

protected:

	Sampler sampler;

};

}