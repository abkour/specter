#include "simd_math.hpp"
#include <immintrin.h>

namespace specter {

vec4f simd_cross(const vec3f& v0, const vec3f& v1) {
	__m128 vec0 = _mm_load_ps(&v0.x);
	__m128 vec1 = _mm_load_ps(&v1.x);
	__m128 c0 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 c1 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 c2 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 c3 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 0, 2, 1));
	vec0 = _mm_sub_ps(_mm_mul_ps(c0, c1), _mm_mul_ps(c2, c3));
	vec4<float> result;
	_mm_store_ps(&result.x, vec0);
	return result;
}

__m128 simd_cross(const __m128& v0, const __m128& v1) {
	__m128 c0 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 c1 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 c2 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 c3 = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(3, 0, 2, 1));
	return _mm_sub_ps(_mm_mul_ps(c0, c1), _mm_mul_ps(c2, c3));
}

}