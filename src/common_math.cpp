#pragma once
#include "common_math.hpp"
#include <cmath>

namespace specter {

float radians(const float angleInDegrees) {
	return angleInDegrees * Pi / 180.f;
}

float log8(const float x) {
	return std::log(x) / std::log(8);
}

unsigned long djb2_hash(unsigned char* str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}


}