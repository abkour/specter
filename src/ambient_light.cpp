#include "ambient_light.hpp"

namespace specter {

vec3f AmbientLight::sample_light(const Accel& accel, const vec3f& point, const vec3f& normal) {
	CoordinateSystem hemisphereFrame(normal);

	vec3f sdir = normalize(uniformlySampleCosineWeightedHemisphere());

	Ray shadowRay(point + normal * 1e-4, hemisphereFrame.toLocal(sdir));	// Displace origin to avoid self-shadowing
	
	return vec3f(0.f);
	//return accel.traceShadowRay(shadowRay) ? vec3f(0.f) : dot(shadowRay.d, normal) * ambient_color;
}

}