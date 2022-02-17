#pragma once
#include "common_math.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include <json.hpp>	

#include <fstream>

namespace specter {

// These hash values were computed via the hash function in common_math.hpp
constexpr uint64_t SPECTER_AMBIENT_LIGHT     = 872128645;
constexpr uint64_t SPECTER_AREA_LIGHT        = 2090087070;
constexpr uint64_t SPECTER_DIRECTIONAL_LIGHT = 2623544947;
constexpr uint64_t SPECTER_POINT_LIGHT       = 271082479;
constexpr uint64_t SPECTER_INVALID           = 0;

struct Scene {

	Scene(const char* filename);

	// Light
	uint64_t lightType;
	vec3f lightEnergy;
	vec3f lightPosition;

	// Camera
	vec3f cameraPosition;
	vec3f cameraTarget;

	// Screen
	vec2u screenResolution;

	// Mesh location
	std::string meshPath;

};

}