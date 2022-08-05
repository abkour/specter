#pragma once
#include "area_light.hpp"
#include "material_lambertian.hpp"
#include "scene.hpp"

namespace specter {

Scene::Scene(SceneDescriptor& sceneDescriptor) {
	
	// Initialize camera
	camera.setResolution(sceneDescriptor.screenResolution);
	camera.initializeVariables(sceneDescriptor.cameraPosition, sceneDescriptor.cameraTarget, sceneDescriptor.cameraFov, sceneDescriptor.samplesPerPixel);

	//  Initialize model
	model = std::make_shared<Model>();
	model->parse(sceneDescriptor.meshPath.c_str());

	// Initialize acceleration structure
	accel.addModel(model);
	accel.build();

	// Set rendering information
	dynamicFrame = sceneDescriptor.dynamicFrame;
	reflection_rays = sceneDescriptor.reflection_rays;
	spp = sceneDescriptor.samplesPerPixel;
}

}