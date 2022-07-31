#include "material.hpp"
#include "texture.hpp"
#include "texture_image.hpp"
#include "texture_single_color.hpp"

namespace specter {

class EmissiveMaterial : public Material {

public:

	EmissiveMaterial(const vec3f& color) 
		: albedo(std::make_shared<SingleColor>(color))
	{}

	EmissiveMaterial(const char* pathname)
		: albedo(std::make_shared<TextureImage>(pathname))
	{}

	virtual vec3f emitted(const float u, const float v, const vec3f& p) {
		return albedo->value(u, v, p);
	}

	virtual bool scatter(const Ray& r_in, const Intersection& its, Ray& r_out, vec3f& attenuation) const override {
		return false;
	}

protected:

	std::shared_ptr<Texture> albedo;

};

}