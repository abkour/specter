#pragma once
#include "rtx/area_light.hpp"
#include "rtx/material_lambertian.hpp"
#include "scene.hpp"
#include "rtx/cpu_lbvh.hpp"

namespace specter {

Scene::Scene(SceneDescriptor& sceneDescriptor) {
	
	// Initialize camera
	camera.setResolution(sceneDescriptor.screenResolution);
	camera.initializeVariables(sceneDescriptor.cameraPosition, sceneDescriptor.cameraTarget, sceneDescriptor.cameraFov, sceneDescriptor.samplesPerPixel);

	//  Initialize model
	model = std::make_shared<Model>();
	model->parse(sceneDescriptor.meshPath.c_str());

	// Initialize acceleration structure
	accel = std::make_shared<Accel>();
	accel->addModel(model);
	accel->build();

	// Integrator
	switch (sceneDescriptor.integrator_type) {
	case IntegratorType::Normal:
		integrator = std::make_shared<NormalIntegrator>();
		break;
	case IntegratorType::PT_Integrator:
		integrator = std::make_shared<PTIntegrator>(sceneDescriptor.reflection_rays);
		break;
	default:
		throw std::runtime_error("Integrator specified in scene descriptor file does not match a known integrator!");
	}

	// Set rendering information
	dynamicFrame = sceneDescriptor.dynamicFrame;
	spp = sceneDescriptor.samplesPerPixel;
}

}