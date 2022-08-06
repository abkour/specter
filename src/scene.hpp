#pragma once
#include "accel.hpp"
#include "ambient_light.hpp"
#include "camera.hpp"
#include "integrator_normal.hpp"
#include "integrator_pt.hpp"
#include "model.hpp"
#include "point_light.hpp"
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