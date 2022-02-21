#include "ambient_light.hpp"

namespace specter {

vec3f AmbientLight::sample_light(const Accel& accel, const vec3f& point, const vec3f& normal) {

	CoordinateSystem hemisphereFrame(normal);

	//auto rayDirection = sampler.uniformlySampleCosineWeightedHemisphere();
	auto rayDirection = sampler.uniformlySampleHemisphere();
	rayDirection = normalize(hemisphereFrame.toLocal(rayDirection));
	
	Ray shadowRay;
	shadowRay.d = rayDirection;
	shadowRay.invd = inverse(shadowRay.d);
	shadowRay.o = point + normal * 1e-4;

	Intersection its;
	if (accel.traceRay(shadowRay, its, true)) {
		return vec3f(0.f);
	}

	float angleTerm = dot(rayDirection, normal) / Pi;
	return angleTerm;
}

}