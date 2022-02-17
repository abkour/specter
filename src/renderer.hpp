#pragma once
#include "accel.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "obj.hpp"
#include "point_light.hpp"
#include "scene_discriptor.hpp"
#include "shader.hpp"
#include "window.hpp"

#include <glad/glad.h>
#include <glfw3.h>

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

namespace specter {

class RTX_Renderer {

public:

	RTX_Renderer(const Scene& scene);
	
	void run();

private:

	Accel accel;
	Camera camera;
	ObjLoader mesh;
	Window window;

};

}