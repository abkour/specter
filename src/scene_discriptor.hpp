#pragma once
#include "common_math.hpp"
#include "vec2.hpp"
#include "vec3.hpp"

#include <json.hpp>	

#include <fstream>
#include <ostream>

namespace specter {

// These hash values were computed via the hash function in common_math.hpp
constexpr uint64_t SPECTER_AMBIENT_LIGHT     = 872128645;
constexpr uint64_t SPECTER_AREA_LIGHT        = 2090087070;
constexpr uint64_t SPECTER_DIRECTIONAL_LIGHT = 2623544947;
constexpr uint64_t SPECTER_POINT_LIGHT       = 271082479;
constexpr uint64_t SPECTER_INVALID           = 0;

constexpr uint64_t SPECTER_DEBUG_DISPLAY_NORMALS = 578727905;
constexpr uint64_t SPECTER_DEBUG_INVALID_MODE = 0;

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