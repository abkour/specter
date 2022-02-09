#include "ray.hpp"

namespace specter {

Ray::Ray(const vec3f& origin, const vec3f& direction)
	: o(origin)
	, d(direction)
	, invd(1.f / direction)
{}

Ray::Ray(const Ray& other)
	: o(other.o)
	, d(other.d)
	, invd(other.invd)
{}

Ray& Ray::operator=(const Ray& other) {
	o = other.o;
	d = other.d;
	invd = other.invd;
	return *this;
}

vec3f Ray::operator()(const float t) {
	return o + (t * d);
}

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
	return os << "origin: " << ray.o << ", direction: " << ray.d;
}

}