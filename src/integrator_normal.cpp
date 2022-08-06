#include "integrator_normal.hpp"

namespace specter {

vec3f NormalIntegrator::render(const Accel& accel, const Ray& ray) {
	Intersection its;
	if (!accel.traceRay(ray, its)) {
		return vec3f(0.f);
	}
	return specter::abs(its.n);
}

}