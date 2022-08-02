/*
	Not used at the moment.
*/
#pragma once
#include <glad/glad.h>

#include "shader.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

namespace specter {

void grayscale_filter(unsigned char* output, const unsigned char* input, const vec2u imageResolution);

}