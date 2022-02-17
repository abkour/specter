#pragma once 
#include "vec3.hpp"
#include "vec4.hpp"

namespace specter {

vec4f simd_cross(const vec3f& v0, const vec3f& v1);
__m128 simd_cross(const __m128& v0, const __m128& v1);

}