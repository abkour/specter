#include "scene_descriptor.hpp"
#include <sstream>

namespace specter {

SceneDescriptor::SceneDescriptor(const char* filename) : filename(filename) {

	std::ifstream file(filename);
	if (file.fail()) {
		throw std::runtime_error("Could not open json file: " + std::string(filename));
	}

	std::stringstream fileContents;
	fileContents << file.rdbuf();
	file.close();

	//
	// 0. Create json parser
	auto jsonParser = nlohmann::json::parse(fileContents.str());
	
	unsigned vec2tmp[2];
	float vec3tmp[3];

	//
	// 1. Debug field
	if (jsonParser.contains("debug")) {
		auto debugParser = jsonParser.find("debug");

		if (debugParser->contains("value")) {
			debugScene = jsonParser["debug"]["value"].get<bool>();
		} 

		if (debugParser->contains("method")) {
			auto debugMode_str = jsonParser["debug"]["method"].get<std::string>();
			debugMode = specter::djb2_hash(reinterpret_cast<unsigned char*>(&debugMode_str[0]));
		}
	}
	
	//
	// Camera field
	if (jsonParser.contains("camera")) {
		auto cameraParser = jsonParser.find("camera");

		if (cameraParser->contains("position")) {
			jsonParser["camera"]["position"].get_to(vec3tmp);
			std::memcpy(&cameraPosition, vec3tmp, sizeof(float) * 3);
		}

		if (cameraParser->contains("target")) {
			jsonParser["camera"]["target"].get_to(vec3tmp);
			std::memcpy(&cameraTarget, vec3tmp, sizeof(float) * 3);
		}

		if (cameraParser->contains("fov")) {
			cameraFov = jsonParser["camera"]["fov"].get<float>();
		}

		if (cameraParser->contains("samples")) {
			samplesPerPixel = jsonParser["camera"]["samples"].get<int>();
		}

		if (cameraParser->contains("resolution")) {
			jsonParser["camera"]["resolution"].get_to(vec2tmp);
			std::memcpy(&screenResolution, vec2tmp, sizeof(unsigned) * 2);
		}
	}

	//
	// Integrator field
	if (jsonParser.contains("integrator")) {
		auto integratorParser = jsonParser.find("integrator");

		if (integratorParser->contains("type")) {
			std::string integrator_name = jsonParser["integrator"]["type"].get<std::string>();
			std::cout << "name: " << integrator_name << '\n';
			if (integrator_name == "PT") {
				integrator_type = IntegratorType::PT_Integrator;
			} else if(integrator_name == "Normal") {
				integrator_type = IntegratorType::Normal;
			} else {
				integrator_type = IntegratorType::None;
			}
		}

		if (integratorParser->contains("traversal_depth")) {
			reflection_rays = jsonParser["integrator"]["traversal_depth"].get<int>();
		}
	}

	//
	// Mesh field
	if (jsonParser.contains("path")) {
		auto meshParser = jsonParser.find("path");
		meshPath = jsonParser["path"].get<std::string>();
	}

	//
	// Misc. field
	if (jsonParser.contains("dynamicFrame")) {
		// Just a place holder for now
		auto plhPraser = jsonParser.find("dynamicFrame");
		dynamicFrame = jsonParser["dynamicFrame"].get<bool>();
	}
}

std::ostream& operator<<(std::ostream& os, const SceneDescriptor& scene) {
	os << "Printing file descriptor: " << scene.filename << '\n';
	os << "cameraPosition: " << scene.cameraPosition << '\n';
	os << "cameraTarget: " << scene.cameraTarget << '\n';
	os << "cameraFov: " << scene.cameraFov << '\n';
	os << "samplesPerPixel: " << scene.samplesPerPixel << '\n';
	os << "screenResolution: " << scene.screenResolution << '\n';
	os << "meshPath: " << scene.meshPath << "\n\n";
	return os;
}

}