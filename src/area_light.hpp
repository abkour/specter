#pragma once
#include "material.hpp"
#include "triangle.hpp"

namespace specter {

class AreaLight : public Material {

public:

	AreaLight() = default;
	
	AreaLight(const vec3f& pAlbedo) : albedo(pAlbedo) {}

	AreaLight(const Triangle& pT0, const Triangle& pT1)
		: t0(pT0)
		, t1(pT1)
	{
		normal = normalize(cross(t0.v1 - t0.v0, t0.v2 - t0.v0));
	}

	vec3f emitted(const float u, const float v, const vec3f& p) override {
		return albedo;
	}

	bool scatter(const Ray& r_in, const Intersection& its, Ray& r_out, vec3f& scattered) const override {
		return false;
	}

protected:

	Triangle t0, t1;
	vec3f albedo;
	vec3f normal;
};

}