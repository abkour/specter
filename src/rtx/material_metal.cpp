#include "material_metal.hpp"
#include "../random_engine.hpp"

namespace specter {

bool Metal::scatter(const Ray& r_in, const Intersection& its, Ray& r_out, vec3f& attenuation) const {
	vec3f reflected = reflect(normalize(r_in.d), its.n);
	r_out = Ray(its.p + reflected * 1e-4, reflected);
	attenuation = albedo;
	return (dot(r_out.d, its.n) > 0);
}

}