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

static specter::MovementDirection getMovementDirection(GLFWwindow* window);

int main(int argc, const char** argv) {
	try {
		static const char* filename = "C:\\Users\\flora\\rsc\\assets\\cube\\cube.obj";
		specter::ObjLoader objLoader(filename);
		const specter::vec3f eyepos(0.f, 0.f, -5.f);
		const specter::vec3f eyetarget(0.f, 0.f, 1.f);
		const specter::vec2u screen_resolution(728, 728);
		specter::Camera camera(screen_resolution);
		camera.initializeVariables(eyepos, eyetarget, 90.f);
		
		std::vector<specter::vec3f> frame;
		frame.resize(specter::product(screen_resolution));

		for (int y = 0; y < screen_resolution.y; ++y) {
			for (int x = 0; x < screen_resolution.x; ++x) {
				const specter::Ray ray = camera.getRay(specter::vec2u(x + 1, y + 1));
				float mint = std::numeric_limits<float>::max();
				unsigned f = std::numeric_limits<unsigned>::max();
				for (int i = 0; i < objLoader.getTriangleCount(); ++i) {
					float u, v, t;
					if (objLoader.rayIntersection(ray, i, u, v, t)) {
						if (t < mint && t > 0.f) {
							mint = t;
							f = i;
						}
					}
				}

				if (f != std::numeric_limits<unsigned>::max()) {
					unsigned normalIndex = objLoader.getFace(f).z;
					specter::vec3f normal = objLoader.getNormal(normalIndex);
					const std::size_t index = y * screen_resolution.x + x;
					frame[index] = normal;
				}
			}
		}

		specter::Window window(specter::WindowMode::WINDOWED, screen_resolution, "Specter Raytracer");
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		window.enableCursorCallback();
		glfwSwapInterval(0);
		glEnable(GL_DEPTH_TEST);

		specter::Shader shader
		{
			{ GL_VERTEX_SHADER, "C:\\Users\\flora\\source\\shaders\\raster\\simple.glsl.vs" },
			{ GL_FRAGMENT_SHADER, "C:\\Users\\flora\\source\\shaders\\raster\\simple.glsl.fs" }
		};
		shader.create();
		shader.bind();

		GLuint rtx_texture;
		glGenTextures(1, &rtx_texture);
		glBindTexture(GL_TEXTURE_2D, rtx_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_resolution.x, screen_resolution.y, 0, GL_RGB, GL_FLOAT, frame.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		// Uncomment when running the RTX shader
		//glBindTextureUnit(0, rtx_texture);

		static const float quad[] = {
			// Vertices		// Texture Coordinates
			-1.f, -1.f,		0.f, 0.f,
			1.f, -1.f,		1.f, 0.f,
			1.f, 1.f,		1.f, 1.f,

			-1.f, -1.f,		0.f, 0.f,
			1.f, 1.f,		1.f, 1.f,
			-1.f, 1.f,		0.f, 1.f
		};
		/*
		GLuint quadvao, quadvbo;
		glGenVertexArrays(1, &quadvao);
		glGenBuffers(1, &quadvbo);
		glBindVertexArray(quadvao);
		glBindBuffer(GL_ARRAY_BUFFER, quadvbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		*/

		unsigned int indices[36];
		for (int i = 0; i < 36; i += 3) {
			indices[i + 0] = objLoader.getFace(i + 0).x;
			indices[i + 1] = objLoader.getFace(i + 1).x;
			indices[i + 2] = objLoader.getFace(i + 2).x;
			std::cout << indices[i + 0] << ' ' << indices[i + 1] << ' ' << indices[i + 2] << '\n';
		}

		std::cout << "vertex count: " << objLoader.getVertexCount() << '\n';

		float oneTriangle[] =
		{
			-1.f, -1.f, 0.f,
			1.f, -1.f, 0.f,
			1.f, 1.f, 0.f,
		};

		GLuint vao, vbo, ebo;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, objLoader.getVertexCount() * sizeof(specter::vec3f), objLoader.getVertices(), GL_STATIC_DRAW);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		specter::View view(specter::vec3f(0.f, 0.f, -3.f), specter::vec3f(0.f, 0.f, 1.f));
		specter::mat4f proj = specter::perspective(specter::radians(45.f), (float)screen_resolution.x / (float)screen_resolution.y, 0.1f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(shader.id(), "proj"), 1, GL_FALSE, &proj.data[0]);
		glUniformMatrix4fv(glGetUniformLocation(shader.id(), "view"), 1, GL_FALSE, view.getAddress());

		float lasttime = 0.f;
		float deltatime = 0.f;

		while (!glfwWindowShouldClose(window.getWindow())) {
			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			deltatime = glfwGetTime() - lasttime;
			lasttime += deltatime;

			if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window.getWindow(), GLFW_TRUE);
			}

			auto movementDirection = getMovementDirection(window.getWindow());
			if (movementDirection != specter::MovementDirection::None) {
				view.move(movementDirection, deltatime);
				std::cout << "Moving to " << view.getPosition() << '\n';
			}

			float cursorXoff = window.getXoffset();
			float cursorYoff = window.getYoffset();
			if (cursorXoff != 0.f || cursorYoff != 0.f) {
				view.look(cursorXoff, cursorYoff);
				window.resetCursorOffset();
			}

			glUniformMatrix4fv(glGetUniformLocation(shader.id(), "proj"), 1, GL_FALSE, &proj.data[0]);
			glUniformMatrix4fv(glGetUniformLocation(shader.id(), "view"), 1, GL_FALSE, view.getAddress());

			glDrawElements(GL_TRIANGLES, objLoader.getTriangleCount() * 3, GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(window.getWindow());
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		//glDeleteTextures(1, &rtx_texture);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	catch (...) {
		std::cout << "Unexpected error somewhere...";
	}
} 

static specter::MovementDirection getMovementDirection(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		return specter::MovementDirection::Forward;
	} else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		return specter::MovementDirection::Backward;
	} else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		return specter::MovementDirection::Left;
	} else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		return specter::MovementDirection::Right;
	}
	return specter::MovementDirection::None;
}