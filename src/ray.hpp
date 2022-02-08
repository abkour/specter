#pragma once
#include "vec3.hpp"

namespace specter {

struct Ray {

	Ray();
	Ray(const vec3f& origin, const vec3f& direction);
	Ray(const Ray& other);
	Ray(Ray&& other);
	Ray& operator=(const Ray& other);

	vec3f operator()(const float t);

	vec3f o, d;
	vec3f invd;	// Inverse direction is used for some intersection algorithms for performance
};

vec3f Ray::operator()(const float t) {
	return o + (t * d);
}

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
	return os << "origin: " << ray.o << ", direction: " << ray.d;
}

}