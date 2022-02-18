/*
	Any non-vector, non-matrix operations go in this file.
*/
#pragma once

namespace specter {

constexpr float Pi = 3.141592653589793;
constexpr float HalfPi = Pi / 2.f;
constexpr float QuarterPi = Pi / 4.f;
constexpr float PiSquared = Pi * Pi;
constexpr float FourPiSquared = 4.f * Pi * Pi;

// Convert angle in degrees to angle in radians
float radians(const float angleInDegrees);

// Compute the logarithm base 8 of x
float log8(const float x);

// Source: http://www.cse.yorku.ca/~oz/hash.html
unsigned long djb2_hash(unsigned char* str);

}