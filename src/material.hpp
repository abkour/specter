#pragma once
#include "intersection.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include <memory>

namespace specter {

class Material {

public:

	Material() = default;

	virtual vec3f GetAlbedo() const {
		return vec3f(0.f);
	}

	virtual vec3f emitted(const float u, const float v, const vec3f& p) {
		return vec3f(0.f);
	}

	virtual bool scatter(const Ray& r_in, const Intersection& its, Ray& r_out, vec3f& attenuation) const = 0;
};

}