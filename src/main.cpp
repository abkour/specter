#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "mat3.hpp"
#include "mat4.hpp"
#include "mesh.hpp"
#include "obj.hpp"
#include "camera.hpp"
#include "common_math.hpp"
#include "window.hpp"
#include "shader.hpp"
#include "view.hpp"
#include "octree.hpp"
#include "accel.hpp"
#include "point_light.hpp"
#include "renderer.hpp"

#include <json.hpp>

static specter::MovementDirection getMovementDirection(GLFWwindow* window);

static const specter::vec2u screen_resolution(1920, 1080);

void renderRasterized();
void renderRTX();

static const unsigned long light_types[] =
{
	271082479
};

// These hash values are computed via the hash function in common_math.hpp
#define SPECTER_AMBIENT_LIGHT		(872128645)		// "ambient"
#define SPECTER_AREA_LIGHT			(2090087070)	// "area"
#define SPECTER_DIRECTIONAL_LIGHT	(2623544947)	// "directional"
#define SPECTER_POINT_LIGHT			(271082479)		// "point"

int main(int argc, const char** argv) {
	try {

		std::ifstream file("C:\\users\\flora\\rsc\\assets\\ajax\\ajax-setup.json");
		std::stringstream fileContents;
		fileContents << file.rdbuf();

		auto jfile = nlohmann::json::parse(fileContents.str());

		auto lightType = jfile["light"]["type"];

		auto lightString = lightType.get<std::string>();
		auto val = specter::djb2_hash(reinterpret_cast<unsigned char*>(&lightString[0]));



		switch (val) {
		case SPECTER_AMBIENT_LIGHT:
			std::cout << "is ambient light";
			break;
		case SPECTER_AREA_LIGHT:
			std::cout << "is area light";
			break;
		case SPECTER_DIRECTIONAL_LIGHT:
			std::cout << "is directional light";
			break;
		case SPECTER_POINT_LIGHT:
			std::cout << "is point light";
			break;
		default:
			std::cout << "is not a light source!";
			break;
		}

		if (!lightType.is_string()) {
			throw std::runtime_error("light/type must specify a light type");
		}
		
		auto ptr = lightType;
		
		if (lightType == "point") {
			std::cout << "Compared\n";
		}
		

		/*
		for (auto j : jfile) {
			std::cout << j << '\n';
		}*/

		//renderRTX();
		//renderRasterized();
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	catch (...) {
		std::cout << "Unexpected error somewhere...";
	}
}

static specter::MovementDirection getMovementDirection(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		return specter::MovementDirection::Forward;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		return specter::MovementDirection::Backward;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		return specter::MovementDirection::Left;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		return specter::MovementDirection::Right;
	}
	return specter::MovementDirection::None;
}

void renderRTX() {
	const specter::vec3f eyepos(-39.8512, 19.539, 14.1864);
	const specter::vec3f eyetarget(eyepos + specter::vec3f(0.78121, -0.0683631, -0.620514));

	specter::RTX_Renderer renderer;
	renderer.loadMesh("C:\\Users\\flora\\rsc\\assets\\ajax\\ajax.obj");
	renderer.initAccel();
	renderer.initCamera(screen_resolution, eyepos, eyetarget);

	renderer.run();
}

void renderRasterized() {
	static const char* filename = "C:\\Users\\flora\\rsc\\assets\\ajax\\ajax.obj";
	specter::ObjLoader mesh;
	mesh.open_read(filename);
	specter::Window window(specter::WindowMode::WINDOWED, screen_resolution, "Specter Rasterizer");
	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	window.enableCursorCallback();
	glfwSwapInterval(0);
	glEnable(GL_DEPTH_TEST);

	auto indices = mesh.getFaces();
	auto vertices = mesh.getVertices();
	auto nIndices = mesh.getTriangleCount() * 3;
	auto nVertices = mesh.getVertexCount();

	std::vector<unsigned> vertexIndices;
	vertexIndices.resize(nIndices);
	for (int i = 0; i < vertexIndices.size(); ++i) {
		vertexIndices[i] = indices[i].p;
	}

	GLuint vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(specter::vec3f), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned), vertexIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	specter::Shader shader
	{
		{ GL_VERTEX_SHADER, "C:\\Users\\flora\\source\\shaders\\raster\\simple.glsl.vs" },
		{ GL_FRAGMENT_SHADER, "C:\\Users\\flora\\source\\shaders\\raster\\simple.glsl.fs" }
	};
	shader.create();
	shader.bind();

	specter::vec3f eyePos(0.f, 0.f, -2.f);
	specter::vec3f eyeDir(0.f);
	specter::View view(eyePos, eyeDir);

	specter::mat4f proj = specter::perspective(specter::radians(45.f), 1.f, 0.1, 100.f);

	float deltatime = 0.f;
	float lasttime = 0.f;

	bool key_c_hit = false;

	while (!glfwWindowShouldClose(window.getWindow())) {
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		deltatime = glfwGetTime() - lasttime;
		lasttime += deltatime;

		if (glfwGetKey(window.getWindow(), GLFW_KEY_C) == GLFW_PRESS) {
			key_c_hit = true;
		}

		if (glfwGetKey(window.getWindow(), GLFW_KEY_C) == GLFW_RELEASE && key_c_hit) {
			std::cout << "pos: " << view.getPosition() << '\n';
			std::cout << "dir: " << view.getDirection() << '\n';
			key_c_hit = false;
		}

		float xoff = window.getXoffset();
		float yoff = window.getYoffset();
		if (xoff != 0.f || yoff != 0.f) {
			view.look(xoff, yoff);
			window.resetCursorOffset();
		}

		auto movementDirection = getMovementDirection(window.getWindow());
		if (movementDirection != specter::MovementDirection::None) {
			view.move(movementDirection, deltatime);
		}

		glUniformMatrix4fv(glGetUniformLocation(shader.id(), "proj"), 1, GL_FALSE, &proj[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader.id(), "view"), 1, GL_FALSE, view.getAddress());

		glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

		if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window.getWindow(), GLFW_TRUE);
		}

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}