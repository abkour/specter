#pragma once
#include "accel.hpp"
#include "common_math.hpp"
#include "coordinate_system.hpp"
#include "light.hpp"
#include "sampler.hpp"

namespace specter {

// The "AmbientLight" class is a special light source in this architecture.
// It has no position or area and is therefore present at every unoccupied 
// point in space. Sampling from the ambient light does not require
// traditional diffuse/specular rays. Instead we perform occlusion queries
// against nearby geometry to get an approximate answer to how much light
// arrives at a given point. This light source should not be seen as a 
// traditional light source and doesn't follow the standard template of 
// a "Light" object.

// The ambient light source is currently deprecated, due to the way lights are incorporated
// into the scene.
class AmbientLight : public ILight {

public:

	AmbientLight(const vec3<uint8_t>& pAmbientColor)
		: ambient_color(pAmbientColor.x / 255.f, pAmbientColor.y / 255.f, pAmbientColor.z / 255.f)
	{}

	vec3f sample_light(const Accel& accel, const vec3f& point, const vec3f& normal) override;

protected:
	
	vec3f ambient_color;
};

}