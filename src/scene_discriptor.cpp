#include "scene_discriptor.hpp"

namespace specter {

Scene::Scene(const char* filename) : filename(filename) {

	std::ifstream file(filename);
	if (file.fail()) {
		throw std::runtime_error("Could not open json file: " + std::string(filename));
	}

	std::stringstream fileContents;
	fileContents << file.rdbuf();
	file.close();
	
	auto jsonFile = nlohmann::json::parse(fileContents.str());

	auto lightType_str = jsonFile["light"]["type"].get<std::string>();
	auto lightType_hash = specter::djb2_hash(reinterpret_cast<unsigned char*>(&lightType_str[0]));

	switch (lightType_hash) {
	case SPECTER_AMBIENT_LIGHT:
		lightType = SPECTER_AMBIENT_LIGHT;
		break;
	case SPECTER_AREA_LIGHT:
		lightType = SPECTER_AREA_LIGHT;
		break;
	case SPECTER_DIRECTIONAL_LIGHT:
		lightType = SPECTER_DIRECTIONAL_LIGHT;
		break;
	case SPECTER_POINT_LIGHT:
		lightType = SPECTER_POINT_LIGHT;
		break;
	default:
		lightType = SPECTER_INVALID;
		break;
	}

	unsigned vec2tmp[2];
	float vec3tmp[3];

	jsonFile["light"]["energy"].get_to(vec3tmp);
	std::memcpy(&lightEnergy, vec3tmp, sizeof(float) * 3);

	jsonFile["light"]["position"].get_to(vec3tmp);
	std::memcpy(&lightPosition, vec3tmp, sizeof(float) * 3);

	jsonFile["camera"]["position"].get_to(vec3tmp);
	std::memcpy(&cameraPosition, vec3tmp, sizeof(float) * 3);

	jsonFile["camera"]["target"].get_to(vec3tmp);
	std::memcpy(&cameraTarget, vec3tmp, sizeof(float) * 3);

	cameraFov = jsonFile["camera"]["fov"].get<float>();

	samplesPerPixel = jsonFile["camera"]["samples"].get<int>();
	
	jsonFile["resolution"].get_to(vec2tmp);
	std::memcpy(&screenResolution, vec2tmp, sizeof(unsigned) * 2);

	meshPath = jsonFile["path"].get<std::string>();
}

}