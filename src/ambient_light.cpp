#include "ambient_light.hpp"

namespace specter {

vec3f AmbientLight::sample_light(const Accel& accel, const vec3f& point, const vec3f& normal) {

	CoordinateSystem hemisphereFrame(normal);

	auto rayDirection = sampler.uniformlySampleCosineWeightedHemisphere();
	rayDirection = normalize(hemisphereFrame.toLocal(rayDirection));
	
	Ray shadowRay;
	shadowRay.d = rayDirection;
	shadowRay.invd = inverse(shadowRay.d);
	shadowRay.o = point;
	
	Intersection its;
	if (!accel.traceRay(shadowRay, its, true)) {
		float angleTerm = dot(rayDirection, normal) / Pi;
		return angleTerm;
	}

	return vec3f(0.f);
}


}