#pragma once
#include "ray.hpp"
#include "vec2.hpp"
#include "vec3.hpp"


namespace specter {

struct Camera {

	Camera(const vec2u resolution);

	// Needs to be called every time either of the three arguments change.
	void initializeVariables(const vec3f& pos, const vec3f& dir, const float fov);

	Ray getRay(const vec2u& pixelLocation);

protected:

	vec2f resolution;
	vec3f eyepos;
	vec3f shiftx, shifty, topLeftPixel;
};

}