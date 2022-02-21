#pragma once
#include "scene.hpp"
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

	RTX_Renderer(Scene* scene);
	
	~RTX_Renderer();

	void run();

private:

	Scene* scene;

	std::vector<specter::vec3f> frame;

	Window window;
	uint64_t debugMode;
};

}