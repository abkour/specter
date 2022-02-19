#pragma once
#include "accel.hpp"
#include "ambient_light.hpp"
#include "camera.hpp"
#include "obj.hpp"
#include "point_light.hpp"
#include "scene_descriptor.hpp"

namespace specter {

struct Scene {

	Scene() = default;
	Scene(SceneDescriptor& sceneDescriptor);

	~Scene();

	Accel accel;
	Camera camera;
	ObjLoader mesh;
	Light* light;
};

}