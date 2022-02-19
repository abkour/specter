#pragma once
#include "accel.hpp"
#include "ambient_light.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "obj.hpp"
#include "point_light.hpp"
#include "scene_descriptor.hpp"
#include "shader.hpp"
#include "window.hpp"

#include <glad/glad.h>
#include <glfw3.h>

#include <tbb/blocked_range.h>
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_for.h>

namespace specter {

class RTX_Renderer {

public:

	RTX_Renderer(const SceneDescriptor& scene);
	
	~RTX_Renderer();

	void run();

private:

	Accel accel;
	Camera camera;
	ObjLoader mesh;
	Window window;

	Light* light;

	int samplesPerPixel;

	uint64_t debugMode;
};

}