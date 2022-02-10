#pragma once
#include "common_math.hpp"

namespace specter {

constexpr float PI = 3.1415926535;

float radians(const float angleInDegrees) {
	return angleInDegrees * PI / 180.f;
}

}