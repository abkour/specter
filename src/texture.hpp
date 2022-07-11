#pragma once
#include "vec3.hpp"

namespace specter {

class Texture {

public:

	virtual vec3f value(const float u, const float v, const vec3f& p) const = 0;
};

}