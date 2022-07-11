#pragma once
#include "vec3.hpp"
#include <memory>

namespace specter {

struct Material;

struct HitRecord {
	vec3f p, n;
	float u, v, t;
	std::shared_ptr<Material> material;
};

}