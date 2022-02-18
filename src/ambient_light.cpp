#include "ambient_light.hpp"

namespace specter {

vec3f AmbientLight::sample_light(const vec3f& point, const vec3f& normal) {

	CoordinateSystem hemisphereFrame(normal);

	auto sample = sampler.uniformlySampleCosineWeightedHemisphere();

	sample = hemisphereFrame.toLocal(sample);
	sample = normalize(sample);

	float cosAngle = dot(sample, normal);



}


}