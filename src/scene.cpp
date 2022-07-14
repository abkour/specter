#pragma once
#include "area_light.hpp"
#include "material_lambertian.hpp"
#include "scene.hpp"

namespace specter {

Scene::Scene(SceneDescriptor& sceneDescriptor) {
	// 1. Initialize Light
	switch (sceneDescriptor.lightType) {
	case SPECTER_AMBIENT_LIGHT:
		light = std::make_shared<AmbientLight>(sceneDescriptor.lightSRGB);
		break;
	case SPECTER_AREA_LIGHT:
		Triangle t0(sceneDescriptor.area_p0, sceneDescriptor.area_p1, sceneDescriptor.area_p2);
		Triangle t1(sceneDescriptor.area_p0, sceneDescriptor.area_p2, sceneDescriptor.area_p3);
		light = std::make_shared<AreaLight>(t0, t1);
		break;
	case SPECTER_DIRECTIONAL_LIGHT:
		throw std::runtime_error("Directional light is not yet supported!");
		break;
	case SPECTER_POINT_LIGHT:
		light = std::make_shared<PointLight>(sceneDescriptor.lightPosition, sceneDescriptor.lightEnergy);
		break;
	default:
		throw std::runtime_error("Light descriptor in scene file doesn't specify a valid light type!");
		break;
	}
	
	// 2. Initialize camera
	camera.setResolution(sceneDescriptor.screenResolution);
	camera.initializeVariables(sceneDescriptor.cameraPosition, sceneDescriptor.cameraTarget, sceneDescriptor.cameraFov, sceneDescriptor.samplesPerPixel);
	camera.setSpp(sceneDescriptor.samplesPerPixel);

	// 3. Initialize model
	model = std::make_shared<Model>();
	model->parse(sceneDescriptor.meshPath.c_str());

	// 4. Initialize acceleration structure
	//accel.addMesh(&mesh);
	accel.addModel(model);
	accel.build();

	// 5. Set rendering information
	dynamicFrame = sceneDescriptor.dynamicFrame;
	reflection_rays = sceneDescriptor.reflection_rays;
}

Scene::~Scene() {}

}