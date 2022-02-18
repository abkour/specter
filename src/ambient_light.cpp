#include "ambient_light.hpp"

namespace specter {

vec3f AmbientLight::sample_light(const vec3f& point, const vec3f& normal) {

	auto sample = sampler.uniformlySampleCosineWeightedHemisphere();

}


}