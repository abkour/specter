#pragma once
#include "intersection.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "window.hpp"

#include <glad/glad.h>
#include <glfw3.h>

#include <tbb/blocked_range.h>
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_for.h>

#include <atomic>
#include <mutex>
#include <thread>

namespace specter {

class RTX_Renderer {

public:

	RTX_Renderer(Scene* scene);
	
	~RTX_Renderer();

	void run();

private:

	void runDynamic();

	// This is function in development.
	void dev_runDynamic();
	vec3f dev_pixel_color(const Ray& ray, int reflectionDepth);

	std::mutex updateMtx;
	bool updateFrame;

	std::atomic<bool> terminateRendering;

	std::thread renderThread;

private:

	Scene* scene;

	std::vector<specter::vec3f> frame;

	GLuint image;

	Window window;
	uint64_t debugMode;
};

}