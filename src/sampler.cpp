/*
	Implementations credited to Physically Based Rendering: From Theory To Implementation 3rd ed,
	13.6 2D Sampling with Multidemnsional Transformations
*/

#include "sampler.hpp"

namespace specter {

constexpr float PI = 3.14159265359;
constexpr float PIHalf = PI / 2.f;
constexpr float PIQuarter = PI / 4.f;

vec2f sampleConcentricDisk() {

	const vec2f p(RandomEngine::get_random_float(), RandomEngine::get_random_float());
	
	vec2f off = 2.f * p - vec2f(1.f);
	if (off.x == 0 && off.y == 0) {
		return vec2f(0.f);
	}
	
	float theta, r;
	if (std::abs(off.x) > std::abs(off.y)) {
		r = off.x;
		theta = PIQuarter * (off.y / off.x);
	} else {
		r = off.y;
		theta = PIHalf - PIQuarter * (off.x / off.y);
	}
	return r * vec2f(std::cos(theta), std::sin(theta));
} 

vec2f uniformlySampleUnitDisk() {
	const vec2f p(RandomEngine::get_random_float(), RandomEngine::get_random_float());
	float r = std::sqrt(p.x);
	float theta = 2 * PI * p.y;
	return vec2f(r * std::cos(theta), r * std::sin(theta));
}

vec3f uniformlySampleHemisphere() {
	const vec2f p(RandomEngine::get_random_float(), RandomEngine::get_random_float());
	float z = 1.f - 2 * p.x;
	float r = std::sqrt(std::max(0.f, 1 - z * z));
	float phi = 2 * PI * p.y;
	return vec3f(r * std::cos(phi), r * std::sin(phi), z);
}

vec3f uniformlySampleCosineWeightedHemisphere() {
	vec2f d = sampleConcentricDisk();
	float y = std::sqrt(std::max(0.f, 1 - d.x * d.x - d.y * d.y));
	return vec3f(d.x, y, d.y);
}

}
