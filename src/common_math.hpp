/*
	Any non-vector, non-matrix operations go in this file.
*/
#pragma once

namespace specter {

// Convert angle in degrees to angle in radians
float radians(const float angleInDegrees);

// Compute the logarithm base 8 of x
float log8(const float x);

// Source: http://www.cse.yorku.ca/~oz/hash.html
unsigned long djb2_hash(unsigned char* str);

}