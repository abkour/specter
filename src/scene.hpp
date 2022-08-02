#pragma once
#include "accel.hpp"
#include "ambient_light.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "point_light.hpp"
#include "scene_descriptor.hpp"

namespace specter {

struct Scene {

	Scene() = default;
	Scene(SceneDescriptor& sceneDescriptor);

	~Scene();

	std::shared_ptr<Model> model;

	Accel accel;
	Camera camera;

	bool dynamicFrame;
	int reflection_rays;
	int spp;
};

}