#pragma once 
#include "vec3.hpp"
#include "vec4.hpp"

namespace specter {

vec4f simd_cross(const vec3f& v0, const vec3f& v1);
__m128 simd_cross(const __m128& v0, const __m128& v1);

template<typename T>
T dot(const vec3<T>& v0, const vec4<T>& v1) {
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

}