#pragma once
#include "material.hpp"
#include "texture_image.hpp"
#include "texture_single_color.hpp"

namespace specter {

class Lambertian : public IMaterial {

public:

	Lambertian(vec3f pAlbedo) 
		: albedo(std::make_shared<SingleColor>(pAlbedo)) 
	{}

	Lambertian(const char* filename)
		: albedo(std::make_shared<TextureImage>(filename))
	{}

	Lambertian(std::shared_ptr<ITexture>& pAlbedo) 
		: albedo(pAlbedo) 
	{}

	vec3f GetAlbedo() const override {
		return albedo->value(0, 0, vec3f(0.f));
	}

	virtual bool scatter(const Ray& r_in, const Intersection& its, Ray& r_out, vec3f& attenuation) const override;

protected:

	std::shared_ptr<ITexture> albedo;
};

}