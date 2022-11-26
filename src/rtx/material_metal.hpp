#pragma once
#include "material.hpp"

namespace specter {

class Metal : public IMaterial {

public:

	Metal(const vec3f& pAlbedo) 
		: albedo(pAlbedo)
	{}

	vec3f GetAlbedo() const override {
		return albedo;
	}

	virtual bool scatter(const Ray& r_in, const Intersection& its, Ray& r_out, vec3f& attenuation) const override;


	vec3f albedo;
};

}