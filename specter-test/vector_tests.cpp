// First time writing unit tests, so this will be bad.
#include "pch.h"

#include "../src/vec2.hpp"
#include "../src/vec3.hpp"
#include "../src/vec4.hpp"

//
// Vec2
TEST(constructor, vec2) {
	specter::vec2i v0(1, 2);
	EXPECT_EQ(v0.x, 1);
	EXPECT_EQ(v0.y, 2);
	specter::vec2i v1(v0);
	EXPECT_EQ(v1.x, 1);
	EXPECT_EQ(v1.y, 2);
	specter::vec2i v2 = v0;
	EXPECT_EQ(v2.x, 1);
	EXPECT_EQ(v2.y, 2);
	specter::vec2i v3 = specter::vec2i(1, 2);
	EXPECT_EQ(v3.x, 1);
	EXPECT_EQ(v3.y, 2);
	specter::vec2i v4(specter::vec2i(1, 2));
	EXPECT_EQ(v4.x, 1);
	EXPECT_EQ(v4.y, 2);
}

TEST(comparison, vec2) {
	specter::vec2i v0(0, 1);
	specter::vec2i v1(0, 1);
	specter::vec2i v2(1, 1);
	specter::vec2i v3(0, 0);

	// operator==
	EXPECT_EQ(v0 == v1, true);
	EXPECT_EQ(v0 == v2, false);
	EXPECT_EQ(v0 == v3, false);

	// operator!=
	EXPECT_EQ(v0 != v1, false);
	EXPECT_EQ(v0 != v2, true);
	EXPECT_EQ(v0 != v3, true);
}

TEST(arithmetic, vec2) {
	specter::vec2i v0(1, 2);
	specter::vec2i v1(10, 20);

	specter::vec2i correct_result_sum(11, 22);
	specter::vec2i v2 = v0 + v1;
	EXPECT_EQ(correct_result_sum == v2, true);

	specter::vec2i correct_result_diff(9, 18);
	specter::vec2i v3 = v1 - v0;
	EXPECT_EQ(correct_result_diff == v3, true);

	specter::vec2i correct_result_prod(10, 40);
	specter::vec2i v4 = v1 * v0;
	EXPECT_EQ(correct_result_prod == v4, true);

	specter::vec2i correct_result_quotient(10, 10);
	specter::vec2i v5 = v1 / v0;
	EXPECT_EQ(correct_result_quotient == v5, true);

	specter::vec2i v00(10, 20);
	specter::vec2i v01(10, 20);
	specter::vec2i v02(10, 20);
	specter::vec2i v03(10, 20);
	v00 += v0;
	v01 -= v0;
	v02 *= v0;
	v03 /= v0;

	EXPECT_EQ(correct_result_sum == v00, true);
	EXPECT_EQ(correct_result_diff == v01, true);
	EXPECT_EQ(correct_result_prod == v02, true);
	EXPECT_EQ(correct_result_quotient == v03, true);
}

TEST(access, vec2) {
	specter::vec2i v0(1, 2);
	EXPECT_EQ(v0[0] == 1, true);

	v0[0] += 1;
	EXPECT_EQ(v0[0] == 2, true);
}


// 
// Vec3
TEST(constructor, vec3) {
	specter::vec3i v0(1, 2, 3);
	EXPECT_EQ(v0.x, 1);
	EXPECT_EQ(v0.y, 2);
	EXPECT_EQ(v0.z, 3);
	specter::vec3i v1(v0);
	EXPECT_EQ(v1.x, 1);
	EXPECT_EQ(v1.y, 2);
	EXPECT_EQ(v1.z, 3);
	specter::vec3i v2 = v0;
	EXPECT_EQ(v2.x, 1);
	EXPECT_EQ(v2.y, 2);
	EXPECT_EQ(v2.z, 3);
	specter::vec3i v3 = specter::vec3i(1, 2, 3);
	EXPECT_EQ(v3.x, 1);
	EXPECT_EQ(v3.y, 2);
	EXPECT_EQ(v3.z, 3);
	specter::vec3i v4(specter::vec3i(1, 2, 3));
	EXPECT_EQ(v4.x, 1);
	EXPECT_EQ(v4.y, 2);
	EXPECT_EQ(v4.z, 3);
}

TEST(comparison, vec3) {
	specter::vec3i v0(0, 1, 2);
	specter::vec3i v1(0, 1, 2);
	specter::vec3i v2(1, 1, 2);
	specter::vec3i v3(0, 0, 2);
	specter::vec3i v4(0, 1, 0);
	specter::vec3i v5(1, 2, 3);

	// ==
	EXPECT_EQ(v0 == v1, true);
	EXPECT_EQ(v0 == v2, false);
	EXPECT_EQ(v0 == v3, false);
	EXPECT_EQ(v0 == v4, false);

	// !=
	EXPECT_EQ(v0 != v1, false);
	EXPECT_EQ(v0 != v2, true);
	EXPECT_EQ(v0 != v3, true);
	EXPECT_EQ(v0 != v4, true);
	
	// <=
	EXPECT_EQ(v0 <= v1, true);
	EXPECT_EQ(v0 <= v5, true);
	
	// <
	EXPECT_EQ(v0 < v5, true);
	EXPECT_EQ(v0 < v2, false);

	// >= 
	EXPECT_EQ(v0 >= v1, true);
	EXPECT_EQ(v0 >= v5, false);

	// >
	EXPECT_EQ(v5 > v0, true);
	EXPECT_EQ(v2 > v0, false);
}

TEST(arithmetic, vec3) {
	specter::vec3i v0(1, 2, 3);
	specter::vec3i v1(10, 20, 30);

	specter::vec3i correct_result_sum(11, 22, 33);
	specter::vec3i v2 = v0 + v1;
	EXPECT_EQ(correct_result_sum == v2, true);

	specter::vec3i correct_result_diff(9, 18, 27);
	specter::vec3i v3 = v1 - v0;
	EXPECT_EQ(correct_result_diff == v3, true);

	specter::vec3i correct_result_prod(10, 40, 90);
	specter::vec3i v4 = v1 * v0;
	EXPECT_EQ(correct_result_prod == v4, true);

	specter::vec3i correct_result_quotient(10, 10, 10);
	specter::vec3i v5 = v1 / v0;
	EXPECT_EQ(correct_result_quotient == v5, true);

	specter::vec3i v00(10, 20, 30);
	specter::vec3i v01(10, 20, 30);
	specter::vec3i v02(10, 20, 30);
	specter::vec3i v03(10, 20, 30);
	v00 += v0;
	v01 -= v0;
	v02 *= v0;
	v03 /= v0;

	EXPECT_EQ(correct_result_sum == v00, true);
	EXPECT_EQ(correct_result_diff == v01, true);
	EXPECT_EQ(correct_result_prod == v02, true);
	EXPECT_EQ(correct_result_quotient == v03, true);
}

TEST(miscellaneous, vec3) {
	specter::vec3i v0(1, 2, 3);
	EXPECT_EQ(specter::minComponent(v0) == 1, true);
	EXPECT_EQ(specter::maxComponent(v0) == 3, true);
	specter::vec3i v1(3, 1, 2);
	EXPECT_EQ(specter::minComponent(v1) == 1, true);
	EXPECT_EQ(specter::maxComponent(v1) == 3, true);
	specter::vec3i v2(2, 3, 1);
	EXPECT_EQ(specter::minComponent(v2) == 1, true);
	EXPECT_EQ(specter::maxComponent(v2) == 3, true);

	specter::vec3i m0 = specter::minComponent(v0, v1);
	specter::vec3i m1 = specter::maxComponent(v0, v1);
	EXPECT_EQ(m0 == specter::vec3i(1, 1, 2), true);
	EXPECT_EQ(m1 == specter::vec3i(3, 2, 3), true);

	specter::vec3i nv(-1, -2, -3);
	EXPECT_EQ(v0 == specter::abs(nv), true);

	EXPECT_EQ(specter::product(nv) == -6, true);

	EXPECT_EQ(specter::sum(nv) == -6, true);

	specter::vec3f lv(0, 3, 4);
	EXPECT_EQ(specter::length(lv) == 5.f, true);

	EXPECT_EQ(specter::length(specter::normalize(lv)) == 1.f, true);
	EXPECT_EQ(specter::normalize(lv) == specter::vec3f(0.0f, 0.6f, 0.8f), true);

	EXPECT_EQ(specter::dot(nv, nv) == 14, true);
	
	EXPECT_EQ(specter::absDot(nv, v0) == 14, true);

	specter::vec3i v4(4, 5, 6);
	EXPECT_EQ(specter::cross(v0, v4) == specter::vec3i(-3, 6, -3), true);

	EXPECT_EQ(invert(nv) == v0, true);

	specter::vec3i v0norm = specter::normalize(v0);

	EXPECT_EQ(specter::reflect(v0, v0norm) == specter::vec3i(1, 2, -3), true);
}

TEST(access, vec3) {
	specter::vec3i v0(1, 2, 3);
	EXPECT_EQ(v0[0] == 1, true);

	v0[0] += 1;
	EXPECT_EQ(v0[0] == 2, true);
}


// 
// Vec4
TEST(constructor, vec4) {
	specter::vec4i v0(1, 2, 3, 4);
	EXPECT_EQ(v0.x, 1);
	EXPECT_EQ(v0.y, 2);
	EXPECT_EQ(v0.z, 3);
	EXPECT_EQ(v0.w, 4);
	specter::vec4i v1(v0);
	EXPECT_EQ(v1.x, 1);
	EXPECT_EQ(v1.y, 2);
	EXPECT_EQ(v1.z, 3);
	EXPECT_EQ(v1.w, 4);
	specter::vec4i v2 = v0;
	EXPECT_EQ(v2.x, 1);
	EXPECT_EQ(v2.y, 2);
	EXPECT_EQ(v2.z, 3);
	EXPECT_EQ(v2.w, 4);
	specter::vec4i v3 = specter::vec4i(1, 2, 3, 4);
	EXPECT_EQ(v3.x, 1);
	EXPECT_EQ(v3.y, 2);
	EXPECT_EQ(v3.z, 3);
	EXPECT_EQ(v3.w, 4);
	specter::vec4i v4(specter::vec4i(1, 2, 3, 4));
	EXPECT_EQ(v4.x, 1);
	EXPECT_EQ(v4.y, 2);
	EXPECT_EQ(v4.z, 3);
	EXPECT_EQ(v4.w, 4);
}

TEST(comparison, vec4) {
	specter::vec4i v0(0, 1, 2, 3);
	specter::vec4i v1(0, 1, 2, 3);
	specter::vec4i v2(1, 1, 2, 3);
	specter::vec4i v3(0, 0, 2, 3);
	specter::vec4i v4(0, 1, 0, 0);
	specter::vec4i v5(1, 2, 3, 4);

	// ==
	EXPECT_EQ(v0 == v1, true);
	EXPECT_EQ(v0 == v2, false);
	EXPECT_EQ(v0 == v3, false);
	EXPECT_EQ(v0 == v4, false);

	// !=
	EXPECT_EQ(v0 != v1, false);
	EXPECT_EQ(v0 != v2, true);
	EXPECT_EQ(v0 != v3, true);
	EXPECT_EQ(v0 != v4, true);

	// <=
	EXPECT_EQ(v0 <= v1, true);
	EXPECT_EQ(v0 <= v5, true);

	// <
	EXPECT_EQ(v0 < v5, true);
	EXPECT_EQ(v0 < v2, false);

	// >= 
	EXPECT_EQ(v0 >= v1, true);
	EXPECT_EQ(v0 >= v5, false);

	// >
	EXPECT_EQ(v5 > v0, true);
	EXPECT_EQ(v2 > v0, false);
}

TEST(arithmetic, vec4) {
	specter::vec4i v0(1, 2, 3, 4);
	specter::vec4i v1(10, 20, 30, 40);

	specter::vec4i correct_result_sum(11, 22, 33, 44);
	specter::vec4i v2 = v0 + v1;
	EXPECT_EQ(correct_result_sum == v2, true);

	specter::vec4i correct_result_diff(9, 18, 27, 36);
	specter::vec4i v3 = v1 - v0;
	EXPECT_EQ(correct_result_diff == v3, true);

	specter::vec4i correct_result_prod(10, 40, 90, 160);
	specter::vec4i v4 = v1 * v0;
	EXPECT_EQ(correct_result_prod == v4, true);

	specter::vec4i correct_result_quotient(10, 10, 10, 10);
	specter::vec4i v5 = v1 / v0;
	EXPECT_EQ(correct_result_quotient == v5, true);

	specter::vec4i v00(10, 20, 30, 40);
	specter::vec4i v01(10, 20, 30, 40);
	specter::vec4i v02(10, 20, 30, 40);
	specter::vec4i v03(10, 20, 30, 40);
	v00 += v0;
	v01 -= v0;
	v02 *= v0;
	v03 /= v0;

	EXPECT_EQ(correct_result_sum == v00, true);
	EXPECT_EQ(correct_result_diff == v01, true);
	EXPECT_EQ(correct_result_prod == v02, true);
	EXPECT_EQ(correct_result_quotient == v03, true);
}

TEST(access, vec4) {
	specter::vec4i v0(1, 2, 3, 4);
	EXPECT_EQ(v0[0] == 1, true);

	v0[0] += 1;
	EXPECT_EQ(v0[0] == 2, true);
}