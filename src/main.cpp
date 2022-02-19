#include "view.hpp"
#include "renderer.hpp"

#include "misc.hpp"

void renderRasterized();
void renderRTX(const char* scene_descriptor_file);

int main(int argc, const char** argv) {
	try {
		std::cout << "specter 3D rendering engine\n\n";
		renderRTX(argv[1]);
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

void renderRasterized() {
	static const char* filename = "C:\\Users\\flora\\rsc\\assets\\ajax\\ajax.obj";
	specter::ObjLoader mesh;
	mesh.parse(filename);
	const specter::vec2u screen_resolution(1920, 1080);
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