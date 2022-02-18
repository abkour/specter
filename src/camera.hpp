#pragma once
#include "ray.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

namespace specter {

struct Camera {

	Camera() = default;

	// Initialize the virtual camera
	Camera(const vec2u resolution);

	// Compute the top-left pixel location of the virtual camera in addition to 
	// the shifting vectors used to shift the top-left pixel.
	// Needs to be called every time either of the three arguments change.
	void initializeVariables(const vec3f& pos, const vec3f& dir, const float fov, const unsigned nSamples);

	// Compute a ray originating from the eye position towards the pixelLocation
	// in world space.
	Ray getRay(const vec2u& pixelLocation);

	vec2u getResolution() const;
	void setResolution(vec2u newResolution);

protected:

	vec2u resolution;
	vec3f eyepos;
	vec3f shiftx, shifty, topLeftPixel;



};

}