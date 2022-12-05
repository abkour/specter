#pragma once
#include "filesystem/model.hpp"
#include "rtx/accel.hpp"
#include "rtx/ambient_light.hpp"
#include "rtx/camera.hpp"
#include "rtx/integrator_normal.hpp"
#include "rtx/integrator_pt.hpp"
#include "rtx/point_light.hpp"
#include "scene_descriptor.hpp"

namespace specter {

struct Scene {

	Scene() = default;
	Scene(SceneDescriptor& sceneDescriptor);

	std::shared_ptr<Model> model;
	std::shared_ptr<IIntegrator> integrator;

	std::shared_ptr<Accel> accel;
	Camera camera;

	bool dynamicFrame;
	int spp;
};

}