#pragma once
#include "vec2.hpp"

#include <glad/glad.h>
#include <glfw3.h>

namespace specter {

enum class WindowMode {
	WINDOWED = 0, BORDERLESS, FULLSCREEN
};

struct Window {

	Window();
	Window(const WindowMode windowMode, const vec2u& resolution, const char* title);
	~Window();

	void enableCursorCallback() const;

	float getXoffset() const;
	float getYoffset() const;

	void resetCursorOffset() const;

	GLFWwindow* getWindow() const;

protected:

	GLFWwindow* window;
	GLFWmonitor* primaryMonitor;
	GLFWvidmode* vidMode;

	WindowMode windowMode;

	vec2u screenResolution;
};

}