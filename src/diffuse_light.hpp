#pragma once
#include "accel.hpp"
#include "light.hpp"
#include "sampler.hpp"

namespace specter {

struct AreaDiffuseLight {

	AreaDiffuseLight() = default;

	vec3f sample_light(const Accel& accel, const vec3f& point, const vec3f& normal);

protected:

	vec3f v0, v1, v2, v3;
	Sampler sampler;
};

}