#pragma once
#include "../math/vec3.hpp"

namespace specter {

class ITexture {

public:

	virtual vec3f value(const float u, const float v, const vec3f& p) const = 0;

	virtual unsigned char* get_data() const {
		return nullptr;
	}
};

}