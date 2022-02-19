#pragma once
#include "scene.hpp"

namespace specter {

Scene::Scene(SceneDescriptor& sceneDescriptor) {

	// 1. Initialize Light
	switch (sceneDescriptor.lightType) {
	case SPECTER_AMBIENT_LIGHT:
		throw std::runtime_error("Ambient light is not yet supported!");
		break;
	case SPECTER_AREA_LIGHT:
		throw std::runtime_error("Area light is not yet supported!");
		break;
	case SPECTER_DIRECTIONAL_LIGHT:
		throw std::runtime_error("Directional light is not yet supported!");
		break;
	case SPECTER_POINT_LIGHT:
		// Make point light
		light = new PointLight(sceneDescriptor.lightPosition, sceneDescriptor.lightEnergy);
		break;
	default:
		throw std::runtime_error("Light descriptor in scene file doesn't specify a valid light type!");
		break;
	}
	
	// 2. Initialize camera
	camera.setResolution(sceneDescriptor.screenResolution);
	camera.initializeVariables(sceneDescriptor.cameraPosition, sceneDescriptor.cameraTarget, sceneDescriptor.cameraFov, sceneDescriptor.samplesPerPixel);

	// 3. Initialize mesh
	mesh.parse(sceneDescriptor.meshPath.c_str());

	accel.addMesh(&mesh);
	accel.build();
}

Scene::~Scene() {
	delete light;
}

}