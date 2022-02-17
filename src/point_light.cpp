#include "point_light.hpp"

namespace specter {

vec3f PointLight::sample_Light(const vec3f& point, const vec3f& normal) const {
	const float FourPiSquared = 39.478417604357434;

	specter::vec3f toLight = this->position - point;
	specter::vec3f toPoint = point - this->position;

	float cosAngle = dot(toLight, normal / specter::length(toLight));

	const float t0 = this->energy.x / FourPiSquared;
	const float t1 = std::max(0.f, cosAngle);
	const float t2 = specter::length(toPoint) * specter::length(toPoint);
	const float t3 = t0 * (t1 / t2);

	return specter::vec3f(t3);
}

}