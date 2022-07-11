#pragma once
#include "accel.hpp"
#include "common_math.hpp"
#include "coordinate_system.hpp"
#include "light.hpp"
#include "sampler.hpp"

namespace specter {

class AmbientLight : public Light {

public:

	AmbientLight(const vec3<uint8_t>& pAmbientColor)
		: ambient_color(pAmbientColor.x / 255.f, pAmbientColor.y / 255.f, pAmbientColor.z / 255.f)
	{
		std::cout << "AmbientColor: ";
		std::cout << (uint32_t)pAmbientColor.x << '\n';
	}

	vec3f sample_light(const Accel& accel, const vec3f& point, const vec3f& normal) override;

protected:
	
	vec3f ambient_color;
	Sampler sampler;
};

}