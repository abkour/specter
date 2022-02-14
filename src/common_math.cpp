#pragma once
#include "common_math.hpp"
#include <cmath>

namespace specter {

constexpr float PI = 3.1415926535;

float radians(const float angleInDegrees) {
	return angleInDegrees * PI / 180.f;
}

float log8(const float x) {
	return std::log(x) / std::log(8);
}


}