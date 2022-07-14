#include "material_lambertian.hpp"
#include "random_engine.hpp"

namespace specter {

bool Lambertian::scatter(const Ray& r_in, const Intersection& its, Ray& r_out, vec3f& attenuation) const {
	attenuation = albedo->value(its.u, its.v, its.p);
	return true;

	vec3f scatter_direction = its.n + RandomEngine::get_random_unit_vector();
	
	// Catch the case where the random unit vector is going in the opposite direction
	// to the surface normal, which results in a null vector.
	// In that case simply use the surface normal as the scatter direction
	if (is_inaccurately_zero(scatter_direction)) {
		scatter_direction = its.n;
	}

	r_out = Ray(its.p, scatter_direction);
	attenuation = albedo->value(its.u, its.v, its.p);
	return true;
}

}