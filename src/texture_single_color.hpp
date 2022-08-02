#pragma once
#include "texture.hpp"

namespace specter {

class SingleColor : public ITexture {

public:

	SingleColor() = default;
	SingleColor(const vec3f& pColor) : color(pColor) {}

	vec3f value(const float u, const float v, const vec3f& p) const override {
		return color;
	}

protected:

	vec3f color;
};

}