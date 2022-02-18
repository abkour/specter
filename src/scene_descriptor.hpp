#pragma once
#include "common.hpp"
#include "common_math.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include <json.hpp>	

#include <fstream>
#include <ostream>

namespace specter {

struct Scene {

	Scene(const char* filename);

	std::string filename;

	// Debug
	bool debugScene;
	uint64_t debugMode;

	// Light
	uint64_t lightType;
	vec3f lightEnergy;
	vec3f lightPosition;

	// Camera
	vec3f cameraPosition;
	vec3f cameraTarget;
	float cameraFov;
	int samplesPerPixel;

	// Screen
	vec2u screenResolution;

	// Mesh location
	std::string meshPath;
};

inline std::ostream& operator<<(std::ostream& os, const Scene& scene) {
	os << "Printing file descriptor: " << scene.filename << '\n';
	os << "lightType: " << scene.lightType << '\n';
	os << "lightEnergy: " << scene.lightEnergy << '\n';
	os << "lightPosition: " << scene.lightPosition << '\n';
	os << "cameraPosition: " << scene.cameraPosition << '\n';
	os << "cameraTarget: " << scene.cameraTarget << '\n';
	os << "cameraFov: " << scene.cameraFov << '\n';
	os << "samplesPerPixel: " << scene.samplesPerPixel << '\n';
	os << "screenResolution: " << scene.screenResolution << '\n';
	os << "meshPath: " << scene.meshPath << "\n\n";
	return os;
}

}