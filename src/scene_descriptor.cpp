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
	
	auto jsonParser = nlohmann::json::parse(fileContents.str());

	debugScene = jsonParser["debug"]["value"].get<int>() == 1 ? true : false;
	auto debugMode_str = jsonParser["debug"]["method"].get<std::string>();
	debugMode = specter::djb2_hash(reinterpret_cast<unsigned char*>(&debugMode_str[0]));

	auto lightType_str = jsonParser["light"]["type"].get<std::string>();
	lightType = specter::djb2_hash(reinterpret_cast<unsigned char*>(&lightType_str[0]));

	unsigned vec2tmp[2];
	float vec3tmp[3];

	jsonParser["light"]["energy"].get_to(vec3tmp);
	std::memcpy(&lightEnergy, vec3tmp, sizeof(float) * 3);

	jsonParser["light"]["position"].get_to(vec3tmp);
	std::memcpy(&lightPosition, vec3tmp, sizeof(float) * 3);

	jsonParser["camera"]["position"].get_to(vec3tmp);
	std::memcpy(&cameraPosition, vec3tmp, sizeof(float) * 3);

	jsonParser["camera"]["target"].get_to(vec3tmp);
	std::memcpy(&cameraTarget, vec3tmp, sizeof(float) * 3);

	cameraFov = jsonParser["camera"]["fov"].get<float>();

	samplesPerPixel = jsonParser["camera"]["samples"].get<int>();
	
	jsonParser["resolution"].get_to(vec2tmp);
	std::memcpy(&screenResolution, vec2tmp, sizeof(unsigned) * 2);

	meshPath = jsonParser["path"].get<std::string>();
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