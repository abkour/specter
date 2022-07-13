#pragma once
#include "common.hpp"
#include "common_math.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include <json.hpp>	

#include <fstream>
#include <ostream>

namespace specter {

struct SceneDescriptor {

	SceneDescriptor(const char* filename);

	std::string filename;

	// Debug
	bool debugScene;
	uint64_t debugMode;

	// Light
	uint64_t lightType;
	
	// Point light specific
	vec3f lightEnergy;
	vec3f lightPosition;

	// Ambient light specific
	vec3<uint8_t> lightSRGB;
	
	// Area light specific
	vec3f area_p0, area_p1, area_p2, area_p3;

	int reflection_rays;

	// Camera
	vec3f cameraPosition;
	vec3f cameraTarget;
	float cameraFov;
	int samplesPerPixel;

	// Screen
	vec2u screenResolution;

	// Mesh location
	std::string meshPath;

	// Rendering
	bool dynamicFrame;

};

std::ostream& operator<<(std::ostream& os, const SceneDescriptor& scene);

}