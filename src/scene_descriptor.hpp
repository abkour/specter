#pragma once
#include "common.hpp"
#include "common_math.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include <json.hpp>	

#include <fstream>
#include <ostream>

namespace specter {

enum class IntegratorType {
	None, Normal, PT_Integrator
};

// Created by parsing a json file containing the scene descriptions.
struct SceneDescriptor {

	SceneDescriptor(const char* filename);

	std::string filename;

	// Debug
	bool debugScene;
	uint64_t debugMode;

	int reflection_rays;

	// Camera
	vec3f cameraPosition;
	vec3f cameraTarget;
	float cameraFov;
	int samplesPerPixel;

	// Integrator
	IntegratorType integrator_type;

	// Screen
	vec2u screenResolution;

	// Mesh location
	std::string meshPath;

	// Rendering
	bool dynamicFrame;
};

std::ostream& operator<<(std::ostream& os, const SceneDescriptor& scene);

}