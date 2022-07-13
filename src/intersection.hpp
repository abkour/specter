#pragma once
#include <limits>
#include "vec2.hpp"
#include "vec3.hpp"

namespace specter {

struct Material;

// Represents an intersection and its corresponding triangle index.
// Also contains coefficient t for ray r(t) := o + t * d
struct Intersection {

	Intersection()
		: u(0.f)
		, v(0.f)
		, f(0)
		, p(0.f)
		, n(0.f)
		, t(std::numeric_limits<float>::max())
	{}

	vec3f p;
	vec3f n;
	float u, v;
	unsigned f;
	float t;

	std::shared_ptr<Material> mat_ptr;

	bool isValid() const {
		return t != std::numeric_limits<float>::max();
	}
};

}