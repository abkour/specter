#include "material_lambertian.hpp"
#include "random_engine.hpp"

namespace specter {

bool Lambertian::scatter(const Ray& r_in, const HitRecord& hit_record, Ray& r_out, vec3f& attenuation) const {
	vec3f scatter_direction = hit_record.n + RandomEngine::get_random_unit_vector();

	// Catch the case where the random unit vector is going in the opposite direction
	// to the surface normal, which results in a null vector.
	// In that case simply use the surface normal as the scatter direction
	if (is_inaccurately_zero(scatter_direction)) {
		scatter_direction = hit_record.n;
	}

	r_out = Ray(hit_record.p, scatter_direction);
	attenuation = albedo->value(hit_record.u, hit_record.v, hit_record.p);
	return true;
}

}