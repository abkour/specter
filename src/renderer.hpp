#pragma once
#include "accel.hpp"
#include "camera.hpp"
#include "mesh.hpp"
#include "obj.hpp"
#include "point_light.hpp"
#include "shader.hpp"
#include "window.hpp"

#include <glad/glad.h>
#include <glfw3.h>

namespace specter {

class RTX_Renderer {

public:

	RTX_Renderer();
	
	void loadMesh(const char* filename);
	void initAccel();
	void initCamera(const vec2u& screenResolution, const vec3f& eyepos, const vec3f& target);

	void run();

private:

	Accel accel;
	Camera camera;
	ObjLoader mesh;
};

}