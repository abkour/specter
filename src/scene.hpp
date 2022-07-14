#pragma once
#include "accel.hpp"
#include "ambient_light.hpp"
#include "camera.hpp"
#include "dev/model.hpp"
#include "obj.hpp"
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
	std::shared_ptr<Light> light;

	bool dynamicFrame;
	int reflection_rays;
};

}