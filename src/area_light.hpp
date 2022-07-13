#pragma once
#include "light.hpp"
#include "triangle.hpp"

namespace specter {

class AreaLight : public Light {

public:

	AreaLight() = default;
	
	AreaLight(const Triangle& pT0, const Triangle& pT1)
		: t0(pT0)
		, t1(pT1)
	{
		normal = normalize(cross(t0.v1 - t0.v0, t0.v2 - t0.v0));
	}

	vec3f sample_light(const Accel& accel, const vec3f& point, const vec3f& normal) override;

protected:

	Triangle t0, t1;
	vec3f normal;
};

}