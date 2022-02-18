#pragma once
#include <random>

namespace specter {

// This object is used as a wrapper over the pseudo-number generation
// facilities provided by the C*+ standard.
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

	float get_next() const {
		return distribution(engine);
	}

private:

	std::mt19937 engine;

	std::uniform_real_distribution<float> distribution;



};


}