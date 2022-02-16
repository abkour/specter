#pragma once
#include "vec2.hpp"
#include "vec3.hpp"

namespace specter {

// Sample the concentric disk
vec2f sampleConcentricDisk(const vec2f& p);
// Sample the unit disk
vec2f uniformlySampleUnitDisk(const vec2f& p);
// Sample the unit hemisphere
vec3f uniformlySampleHemisphere(const vec2f& p);
// Sample the cosine weighted unit sphere
vec3f uniformlySampleCosineWeightedHemisphere(const vec2f& p);

}