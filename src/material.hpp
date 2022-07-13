#pragma once
#include "intersection.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include <memory>

namespace specter {

class Material {

public:

	Material() = default;

	virtual bool scatter(const Ray& r_in, const Intersection& its, Ray& r_out, vec3f& attenuation) const = 0;

};

}