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

void Camera::initializeVariables(const vec3f& pos, const vec3f& dir, const float fov) {
	eyepos = pos;
	const vec3f T(dir - pos);
	const vec3f up(0.f, 1.f, 0.f);
	const vec3f right_norm(normalize(cross(up, T)));
	const vec3f t_norm(normalize(T));
	const vec3f up_norm(cross(t_norm, right_norm));
	const float aspect_ratio = (resolution.x - 1) / (resolution.y - 1);
	const float gx = std::tan(radians(fov / 2.f));
	const float gy = gx * ((resolution.y - 1) / (resolution.x - 1));
	
	shiftx = right_norm * ((2 * gx) / (resolution.x - 1));
	shifty = up_norm * ((2 * gy) / (resolution.y - 1));
	topLeftPixel = t_norm - (right_norm * gx) - (up_norm * gy);
}

Ray Camera::getRay(const vec2u& pixelLocation) {
	vec3f origin = eyepos;
	vec3f direction = topLeftPixel + (shiftx * (pixelLocation.x - 1)) + (shifty * (pixelLocation.y - 1));
	return Ray(origin, direction);
}

}