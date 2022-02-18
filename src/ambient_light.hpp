#pragma once
#include "accel.hpp"
#include "common_math.hpp"
#include "coordinate_system.hpp"
#include "light.hpp"
#include "sampler.hpp"

namespace specter {

struct AmbientLight {

	AmbientLight() = default;

	vec3f sample_light(const Accel& accel, const vec3f& point, const vec3f& normal);

protected:

	Sampler sampler;
};

}