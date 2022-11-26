#pragma once
#include "material.hpp"
#include "triangle.hpp"

namespace specter {

class AreaLight : public IMaterial {

public:

	AreaLight() = default;
	
	AreaLight(const vec3f& pAlbedo) : albedo(pAlbedo) {}

	vec3f emitted(const float u, const float v, const vec3f& p) override {
		return albedo;
	}

	bool scatter(const Ray& r_in, const Intersection& its, Ray& r_out, vec3f& scattered) const override {
		return false;
	}

protected:

	vec3f albedo;
};

}