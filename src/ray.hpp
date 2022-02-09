#pragma once
#include "vec3.hpp"

namespace specter {

struct Ray {

	Ray() = default;
	Ray(const vec3f& origin, const vec3f& direction);
	Ray(const Ray& other);
	Ray& operator=(const Ray& other);

	vec3f operator()(const float t);

	vec3f o, d;
	vec3f invd;	// Inverse direction is used for some intersection algorithms for performance
};

}