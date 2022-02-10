#pragma once
#include "window.hpp" 

namespace specter {

Window::Window() {
	if (!glfwInit()) {
		throw std::runtime_error("Error. GLFW could not be initialized!");
	}
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_PROFILE);
	glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);

	primaryMonitor = glfwGetPrimaryMonitor();
	if (primaryMonitor == nullptr) {
		throw std::runtime_error("Error. GLFW could not detect any monitor!");
	}

	vidMode = const_cast<GLFWvidmode*>(glfwGetVideoMode(primaryMonitor));

	screenResolution.x = vidMode->width;
	screenResolution.y = vidMode->height;

	windowMode = WindowMode::WINDOWED;

	window = glfwCreateWindow(screenResolution.x, screenResolution.y, "Specter", NULL, NULL);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Error. GLAD could not be loaded!");
	}
}

Window::Window(const WindowMode windowMode, const vec2u& resolution, const char* title) {
	if (!glfwInit()) {
		throw std::runtime_error("Error. GLFW could not be initialized!");
	}
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_PROFILE);
	glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);

	primaryMonitor = glfwGetPrimaryMonitor();
	if (primaryMonitor == nullptr) {
		throw std::runtime_error("Error. GLFW could not detect any monitor!");
	}

	vidMode = const_cast<GLFWvidmode*>(glfwGetVideoMode(primaryMonitor));

	switch (windowMode) {
	case WindowMode::WINDOWED:
		window = glfwCreateWindow(resolution.x, resolution.y, title, NULL, NULL);
		break;
	case WindowMode::BORDERLESS:
		glfwWindowHint(GLFW_RED_BITS, vidMode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, vidMode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, vidMode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, vidMode->refreshRate);
		window = glfwCreateWindow(resolution.x, resolution.y, title, primaryMonitor, NULL);
		break;
	case WindowMode::FULLSCREEN:
		window = glfwCreateWindow(resolution.x, resolution.y, title, primaryMonitor, NULL);
		break;
	default:
		throw std::invalid_argument("Error. (Note to programmer) WindowMode structure should not contain cases outside the once handled from this location!");
		break;
	}

	screenResolution = resolution;
	this->windowMode = windowMode;

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Error. GLAD could not be loaded!");
	}
}


Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* Window::getWindow() const {
	return window;
}

}