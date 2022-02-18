#pragma once
#include "vec2.hpp"
#include "vec3.hpp"
#include "random_engine.hpp"

namespace specter {

struct Sampler {

	// Default construct the random engine
	Sampler();

	// Sample the concentric disk
	vec2f sampleConcentricDisk();
	// Sample the unit disk
	vec2f uniformlySampleUnitDisk();
	// Sample the unit hemisphere
	vec3f uniformlySampleHemisphere();
	// Sample the cosine weighted unit sphere
	vec3f uniformlySampleCosineWeightedHemisphere();

private:

	RandomEngine randomEngine;

};


}