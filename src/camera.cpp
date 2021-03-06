#pragma once
#include "camera.hpp"
#include "common_math.hpp"

namespace specter {

Camera::Camera(const vec2u resolution) 
	: resolution(resolution)
	, eyepos(0)
	, shiftx(0)
	, shifty(0)
	, topLeftPixel(0)
{}

void Camera::initializeVariables(const vec3f& pos, const vec3f& dir, const float fov, const unsigned nSamples) {
	eyepos = pos;
	const vec3f T(dir - pos);
	const vec3f up(0.f, 1.f, 0.f);
	const vec3f right_norm(normalize(cross(up, T)));
	const vec3f t_norm(normalize(T));
	const vec3f up_norm(cross(t_norm, right_norm));
	const float aspect_ratio = ((float)resolution.x - 1) / ((float)resolution.y - 1);
	const float gx = std::tan(radians(fov / 2.f));
	const float gy = gx * (((float)resolution.y - 1) / ((float)resolution.x - 1));
	
	float samplesPerDirection = std::sqrt(nSamples);

	shiftx = right_norm * ((2 * gx) / (((float)resolution.x - 1) * samplesPerDirection));
	shifty = up_norm * ((2 * gy) / (((float)resolution.y - 1) * samplesPerDirection));
	topLeftPixel = t_norm - (right_norm * gx) - (up_norm * gy);
}

Ray Camera::getRay(const vec2u& pixelLocation) {
	vec3f origin = eyepos;
	vec3f direction = topLeftPixel + (shiftx * (pixelLocation.x - 1)) + (shifty * (pixelLocation.y - 1));
	return Ray(origin, normalize(direction));
}

void Camera::setResolution(vec2u newResolution) {
	resolution = newResolution;
}

unsigned Camera::resx() const {
	return resolution.x;
}

unsigned Camera::resy() const {
	return resolution.y;
}

void Camera::setSpp(const unsigned samplesPerPixel) {
	unsigned sqrt_x = sqrtf(samplesPerPixel);
	if (sqrt_x * sqrt_x != samplesPerPixel) {
		throw std::runtime_error("Error. Specified number of samples has to be a square number!");
	}
	this->samplesPerPixel = samplesPerPixel;
}

unsigned Camera::spp() const {
	return samplesPerPixel;
}

}