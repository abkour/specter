#pragma once
#include "vec2.hpp"

#include <glad/glad.h>
#include <glfw3.h>

namespace specter {

enum class WindowMode {
	WINDOWED = 0, BORDERLESS, FULLSCREEN
};

// Wrapper over GLFW
struct Window {

	// Default initialize the window. Sets the window resolution to the resolution of the primary monitor
	Window();
	// Initialize the window and use the window mode, resolution and title of the window.
	Window(const WindowMode windowMode, const vec2u& resolution, const char* title);
	// Destroy this instance and release GLFW resources
	~Window();

	// Enable the cursor in the window
	void enableCursorCallback() const;

	// Get the relative distance the cursor moved in the x direction, since the last call of this function
	float getXoffset() const;
	
	// Get the relative distance the cursor moved in the y direction, since the last call of this function
	float getYoffset() const;

	// Reset the cursor offset position
	void resetCursorOffset() const;

	// Get the underlying window object
	GLFWwindow* getWindow() const;

protected:

	GLFWwindow* window;	// Represents the window object
	GLFWmonitor* primaryMonitor;	// Represents the primary monitor
	GLFWvidmode* vidMode;	// Represents the video mode of the primary monitor

	WindowMode windowMode;	// Represents the mode of the window 

	vec2u screenResolution;	// Specifies the window resolution
};

}