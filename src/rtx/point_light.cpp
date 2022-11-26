#include "point_light.hpp"

namespace specter {

vec3f PointLight::sample_light(const Accel& accel, const vec3f& point, const vec3f& normal) {
	const float FourPiSquared = 39.478417604357434;

	specter::vec3f toLight = position - point;
	specter::vec3f toPoint = point - position;

	float cosAngle = dot(toLight, normal / specter::length(toLight));

	const float t0 = dot(toPoint, toPoint);
	const float t1 = std::max(0.f, cosAngle);

	specter::vec3f res;
	for (int i = 0; i < 3; ++i) {
		const float t2 = energy[i] / FourPiSquared;
		const float t3 = t2 * (t1 / t0);
		res[i] = t3;
	}

	return res;
}

}