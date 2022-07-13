#pragma once
#include "triangle.hpp"

namespace specter {

bool Triangle::intersect(const Ray& r_in, Intersection& its) {
	const float epsilon = 1e-7;

	vec3f e0 = v1 - v0;
	vec3f e1 = v2 - v0;

	vec3f q = cross(r_in.d, e1);
	float a = dot(e0, q);

	if (a > -epsilon && a < epsilon) {
		return false;
	}

	float f = 1.f / a;
	vec3f s = r_in.o - v0;
	its.u = f * dot(s, q);

	if (its.u < 0.f) return false;

	vec3f r = cross(s, e0);
	its.v = f * dot(r_in.d, r);

	if (its.v < 0.f || its.u + its.v > 1.f) return false;

	its.t = f * dot(e1, r);
	return true;
}

}