#include "scene_descriptor.hpp"

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
	// 2. Light field
	if (jsonParser.contains("light")) {
		auto lightParser = jsonParser.find("light");

		if (lightParser->contains("type")) {
			auto lightType_str = jsonParser["light"]["type"].get<std::string>();
			lightType = specter::djb2_hash(reinterpret_cast<unsigned char*>(&lightType_str[0]));
		}

		if (lightType == SPECTER_AMBIENT_LIGHT) {
			if (lightParser->contains("srgb")) {
				uint8_t u8x3[3];
				jsonParser["light"]["srgb"].get_to(u8x3);
				std::memcpy(&lightSRGB, u8x3, sizeof(uint8_t) * 3);
				std::cout << u8x3[0] << ", " << u8x3[1] <<  ", " << u8x3[2] << '\n';
			}
			if (lightParser->contains("reflection_rays")) {
				reflection_rays = jsonParser["light"]["reflection_rays"].get<int>();
			}
		} else {
			if (lightParser->contains("position")) {
				jsonParser["light"]["position"].get_to(vec3tmp);
				std::memcpy(&lightPosition, vec3tmp, sizeof(float) * 3);
			}
		}
	}

	//
	// 3. Camera field
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
	// 4. Mesh field
	if (jsonParser.contains("path")) {
		auto meshParser = jsonParser.find("path");
		meshPath = jsonParser["path"].get<std::string>();
	}

	//
	// 5. Misc. field
	if (jsonParser.contains("dynamicFrame")) {
		// Just a place holder for now
		auto plhPraser = jsonParser.find("dynamicFrame");
		dynamicFrame = jsonParser["dynamicFrame"].get<bool>();
	}
}

static std::string lightTypeToString(const uint64_t lightType) {
	std::string lightName;
	switch (lightType) {
	case SPECTER_AMBIENT_LIGHT:
		lightName = "Ambient";
		break;
	case SPECTER_AREA_LIGHT:
		lightName = "Area";
		break;
	case SPECTER_DIRECTIONAL_LIGHT:
		lightName = "Directional";
		break;
	case SPECTER_POINT_LIGHT:
		lightName = "Point";
		break;
	default:
		lightName = "Invalid";
		break;
	}
	return lightName;
}

std::ostream& operator<<(std::ostream& os, const SceneDescriptor& scene) {
	os << "Printing file descriptor: " << scene.filename << '\n';
	os << "lightType: " << lightTypeToString(scene.lightType) << '\n';
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