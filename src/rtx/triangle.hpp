#pragma once
#include "intersection.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace specter {

class Triangle {

public:

	Triangle() = default;
	Triangle(const vec3f& pV0, const vec3f& pV1, const vec3f& pV2)
		: v0(pV0)
		, v1(pV1)
		, v2(pV2)
	{}

	bool intersect(const Ray& r_in, Intersection& hit_record);

	vec3f v0, v1, v2;
};

}
