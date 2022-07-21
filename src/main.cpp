#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(disable:4996)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "view.hpp"
#include "renderer.hpp"

#include "misc.hpp"

#include "filters.hpp"

void testFilters();
void renderRasterized(const char* filename);
void renderRTX(const char* scene_descriptor_file);

#include <bitset>
#include <xmmintrin.h>

int main(int argc, const char** argv) {
	try {
		std::cout << "specter 3D rendering engine\n\n";
		renderRTX(argv[1]);
		//renderRasterized(argv[1]);
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what();
	}
	catch (...) {
		std::cout << "Unexpected error somewhere...";
	}
}

void renderRTX(const char* scene_descriptor_file) {
	specter::SceneDescriptor scene_descriptor(scene_descriptor_file);
	specter::Scene scene(scene_descriptor);
	specter::RTX_Renderer renderer(&scene);
	renderer.run();
}

static specter::MovementDirection getMovementDirection(GLFWwindow* window);

void renderRasterized(const char* scene_descriptor_file) {
	specter::SceneDescriptor scene_descriptor(scene_descriptor_file);
	specter::Scene scene(scene_descriptor);
	
	const specter::vec2u screen_resolution(1920, 1080);
	specter::Window window(specter::WindowMode::WINDOWED, screen_resolution, "Specter Rasterizer");
	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	window.enableCursorCallback();
	glfwSwapInterval(0);
	glEnable(GL_DEPTH_TEST);

	auto indices = scene.model->GetFaces();
	auto vertices = scene.model->GetVertices();
	auto nIndices = scene.model->GetFaceCount();
	auto nVertices = scene.model->GetVertexCount();

	for (int i = 0; i < scene.model->GetNumberOfMeshes(); ++i) {
		std::cout << "Mesh: " << scene.model->GetMeshName(i) << '\n';
	}

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

	specter::mat4f proj = specter::perspective(specter::radians(45.f), 1920.f / 1080.f, 0.1, 100.f);

	float deltatime = 0.f;
	float lasttime = 0.f;

	bool key_c_hit = false;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
			std::cout << "target: " << view.getPosition() + view.getDirection() << '\n';
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

void testFilters() {
	specter::vec2u screen_resolution(1920, 1080);
	specter::Window window(specter::WindowMode::WINDOWED, screen_resolution, "Filter test");

	int width, height, nrChannels;
	unsigned char* data = stbi_load("C:\\Users\\flora\\source\\repos\\specter\\results\\dev\\random.png", &width, &height, &nrChannels, 4);
	if (data) {
		unsigned char* output_image = new unsigned char[width * height * 4];
		specter::Timer filtertime;
		specter::grayscale_filter(output_image, data, specter::vec2u(width, height));
		std::cout << "FIltering took: " << filtertime.elapsedTime() << '\n';
		stbi_write_png(	"C:\\Users\\flora\\source\\repos\\specter\\results\\dev\\gray_random.png", 
						width, 
						height,
						4,
						output_image, 
						width * 4);
		delete[] output_image;
	} else {
		std::cout << "Failed reading data!\n";
	}
	stbi_image_free(data);
}