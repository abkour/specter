#pragma once
#include "vec3.hpp"
#include <random>

namespace specter {

// This object is used as a wrapper over the pseudo-number generation
// facilities provided by the C++ standard.
// It cannot be made const, because the internal state of the 
// random engine changes when a number is generated
class RandomEngine {

public:

	// Common case
	RandomEngine()
		: engine()
		, distribution(0.f, 1.f)
	{}

	// Specialized case
	RandomEngine(const std::size_t custom_seed, float low, float high) 
		: engine(custom_seed)
		, distribution(low, high)
	{}

	float get_next() {
		return distribution(engine);
	}

	// In range [0.0, 1.0)
	static float get_random_float() {
		return rand() / (RAND_MAX + 1.f);
	}

	static float get_random_float(float rmin, float rmax) {
		return rmin + (rmax - rmin) * get_random_float();
	}

	static vec3f get_random_vector(float rmin, float rmax) {
		return vec3f(	get_random_float(rmin, rmax),
						get_random_float(rmin, rmax),
						get_random_float(rmin, rmax));
	}

	// Use simple rejection method
	static vec3f get_random_point_in_unit_sphere() {
		while (true) {
			vec3f p = get_random_vector(-1, 1);
			if (dot(p, p) >= 1) continue;
			return p;
		}
	}

	static vec3f get_random_unit_vector() {
		return normalize(get_random_point_in_unit_sphere());
	}

private:

	std::mt19937 engine;
	std::uniform_real_distribution<float> distribution;
};


}