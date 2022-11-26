#include "area_light.hpp"

namespace specter {

/*
vec3f AreaLight::scatter(const Accel& accel, const vec3f& point, const vec3f& normal) {
	// We need to perform multiple shadow queries on the area light source.
	// For now, we are doing a fixed number of queries, but this number can
	// be adjusted based on the surface area of the light source (larger areas
	// need fewer queries).

	// For now, we simply perform a query against the center of the area light source.
	// Once we learn more about sampling techniques, this will become more sophisticated.
	// Note: It is assumed that the area is a polygon that follows this equality.
	constexpr float epsilon = 1e-3;
	vec3f area_center = t0.v0 + ((t0.v2 - t0.v0) / 2.f);
	vec3f sdir = normalize(area_center - (point + normal * epsilon));
	Ray shadowRay(point + normal * epsilon, sdir);	// Displace origin to avoid self-shadowing
	
	float tmax = (area_center.x - shadowRay.o.x) / sdir.x;

	return accel.traceShadowRayTmax(shadowRay, tmax) ? vec3f(0.f) : vec3f(1.f);
}*/
}