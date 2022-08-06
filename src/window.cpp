#pragma once
#include "window.hpp" 

namespace specter {

Window::Window() = default;

Window::Window(const WindowMode windowMode, const vec2u& resolution, const char* title) {
	if (!glfwInit()) {
		throw std::runtime_error("Error. GLFW could not be initialized!");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);

	primaryMonitor = glfwGetPrimaryMonitor();
	if (!primaryMonitor) {
		throw std::runtime_error("Error. GLFW could not detect any monitor!");
	}

	vidMode = const_cast<GLFWvidmode*>(glfwGetVideoMode(primaryMonitor));
	if (!vidMode) {
		throw std::runtime_error("Error. GLFW coudln't retrieve video mode of current monitor!");
	}

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

	if (!window) {
		throw std::runtime_error("Error. GLFW could not create window!");
	}

	screenResolution = resolution;
	this->windowMode = windowMode;

	glfwMakeContextCurrent(window);
	if (glfwGetError(NULL)) {
		throw std::runtime_error("Error. GLFW could not make context current!");
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Error. GLAD could not be loaded!");
	}
}

void Window::openWindow(const WindowMode windowMode, const vec2u& resolution, const char* title) {
	if (window != nullptr) {
		throw std::runtime_error("This winodw has already been initialized!");
	}

	if (!glfwInit()) {
		throw std::runtime_error("Error. GLFW could not be initialized!");
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);

	primaryMonitor = glfwGetPrimaryMonitor();
	if (!primaryMonitor) {
		throw std::runtime_error("Error. GLFW could not detect any monitor!");
	}

	vidMode = const_cast<GLFWvidmode*>(glfwGetVideoMode(primaryMonitor));
	if (!vidMode) {
		throw std::runtime_error("Error. GLFW coudln't retrieve video mode of primary monitor!");
	}

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

	if (!window) {
		throw std::runtime_error("Error. GLFW could not create window!");
	}

	screenResolution = resolution;
	this->windowMode = windowMode;

	glfwMakeContextCurrent(window);
	if (glfwGetError(NULL)) {
		throw std::runtime_error("Error. GLFW could not make context current!");
	}

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

struct Cursor {

	Cursor(float xpos, float ypos) 
		: xpos(xpos)
		, ypos(ypos)
		, xoff(0.f)
		, yoff(0.f)
		, cursorZoom(0.f)
	{}

	float xpos;
	float ypos;
	float xoff;
	float yoff;

	float cursorZoom;

	bool initialEntry = true;
};

static Cursor uniqueCursor(0.f, 0.f);

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	if (uniqueCursor.initialEntry) {
		uniqueCursor.xpos = xpos;
		uniqueCursor.ypos = ypos;
		uniqueCursor.initialEntry = false;
	}

	uniqueCursor.xoff = xpos - uniqueCursor.xpos;
	uniqueCursor.yoff = uniqueCursor.ypos - ypos;
	uniqueCursor.xpos = xpos;
	uniqueCursor.ypos = ypos;
}

void cursorZoomCallback(GLFWwindow* window, double xoffset, double yoffset) {
	uniqueCursor.cursorZoom += yoffset;
}

void Window::enableCursorCallback() const {
	glfwSetCursorPosCallback(window, cursorPositionCallback);
}

void Window::enableCursorZoom() const {
	glfwSetScrollCallback(window, cursorZoomCallback);
}

float Window::getCursorZoom() const {
	return uniqueCursor.cursorZoom;
}

float Window::getXoffset() const {
	return uniqueCursor.xoff;
}

float Window::getYoffset() const {
	return uniqueCursor.yoff;
}

void Window::resetCursorOffset() const {
	uniqueCursor.xoff = 0.f;
	uniqueCursor.yoff = 0.f;
}

enum KeyState {
	Key_Released = 0,
	Key_Pressed = 1
};

struct Keyboard {

	Keyboard()
		: Key_W(Key_Released)
		, Key_S(Key_Released)
		, Key_A(Key_Released)
		, Key_D(Key_Released)
	{}

	KeyState Key_W;
	KeyState Key_S;
	KeyState Key_A;
	KeyState Key_D;
};

static Keyboard uniqueKeyboard;

void keyStateCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		uniqueKeyboard.Key_W = Key_Pressed;
	} else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		uniqueKeyboard.Key_S = Key_Pressed;
	} else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		uniqueKeyboard.Key_A = Key_Pressed;
	} else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		uniqueKeyboard.Key_D = Key_Pressed;
	}

	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		uniqueKeyboard.Key_W = Key_Released;
	} else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		uniqueKeyboard.Key_S = Key_Released;
	} else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		uniqueKeyboard.Key_A = Key_Released;
	} else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		uniqueKeyboard.Key_D = Key_Released;
	}
}

void Window::enableKeyStateCallback() const {
	glfwSetKeyCallback(window, keyStateCallback);
}

}