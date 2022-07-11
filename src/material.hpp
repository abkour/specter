#pragma once
#include "hit_record.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include <memory>

namespace specter {

class Material {

public:

	Material();

	virtual bool scatter(const Ray& r_in, const HitRecord& hit_record, Ray& r_out, vec3f& attenuation) const;

};

}