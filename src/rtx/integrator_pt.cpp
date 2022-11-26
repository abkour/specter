#include "integrator_pt.hpp"

namespace specter {

vec3f PTIntegrator::render(const Accel& accel, const Ray& ray) {
	return render_rec(accel, ray, pathDepth);
}

vec3f PTIntegrator::render_rec(const Accel& accel, const Ray& ray, int traversal_depth) {
	if (traversal_depth <= 0) {
		return vec3f(0.f);
	}

	Intersection its;
	if (!accel.traceRay(ray, its)) {
		return vec3f(0.f);
	}

	Ray scattered;
	vec3f attenuation;
	vec3f emitted = its.mat_ptr->emitted(its.u, its.v, its.p);
	if (!its.mat_ptr->scatter(ray, its, scattered, attenuation)) {
		return emitted;
	}

	return emitted + attenuation * render_rec(accel, scattered, traversal_depth - 1);
}

}