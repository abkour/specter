#pragma once
#include "vec2.hpp"
#include "vec3.hpp"

namespace specter {

vec2f sampleConcentricDisk(const vec2f& p);
vec2f uniformlySampleUnitDisk(const vec2f& p);
vec3f uniformlySampleHemisphere(const vec2f& p);
vec3f uniformlySampleCosineWeightedHemisphere(const vec2f& p);

}