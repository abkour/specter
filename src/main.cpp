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

int main(int argc, const char** argv) {
	try {
		static const char* filename = "C:\\Users\\flora\\rsc\\assets\\cube\\cube.obj";
		specter::ObjLoader objLoader(filename);
		const specter::vec3f eyepos(0.f, 0.f, -1.5f);
		const specter::vec3f eyetarget(0.f, 0.f, 1.f);
		const specter::vec2u screen_resolution(728, 728);
		const float fov = specter::radians(45.f);
		specter::Camera camera(screen_resolution);
		camera.initializeVariables(eyepos, eyetarget, fov);
		
		std::vector<specter::vec3f> frame;
		frame.resize(specter::product(screen_resolution));

		for (int x = 0; x < screen_resolution.x; ++x) {
			for (int y = 0; y < screen_resolution.y; ++y) {
				const std::size_t index = y * screen_resolution.x + x;
				specter::Ray ray = camera.getRay(specter::vec2u(x + 1, y + 1));
				
				float mint = std::numeric_limits<float>::max();
				unsigned f = std::numeric_limits<unsigned>::max();

				for (int i = 0; i < objLoader.getTriangleCount(); ++i) {
					float u, v, t;
					if (objLoader.rayIntersection(ray, i, u, v, t)) {
						if (mint > t) {
							mint = t;
							f = i;
						}
					}
				}

				if (f != std::numeric_limits<unsigned>::max()) {
					specter::vec3f normal = objLoader.getNormal(f);
					frame[index] = normal;
				}
			}
		}

		specter::Window window(specter::WindowMode::WINDOWED, screen_resolution, "sometitle");

		GLuint rtx_texture;
		glGenTextures(1, &rtx_texture);
		glBindTexture(GL_TEXTURE_2D, rtx_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_resolution.x, screen_resolution.y, 0, GL_RGB, GL_FLOAT, frame.data());
		glGenerateMipmap(GL_TEXTURE_2D);

		static const float quad[] = {
			// Vertices		// Texture Coordinates
			-1.f, -1.f,		0.f, 0.f,
			1.f, -1.f,		1.f, 0.f,
			1.f, 1.f,		1.f, 1.f,

			-1.f, -1.f,		0.f, 0.f,
			1.f, 1.f,		1.f, 1.f,
			-1.f, 1.f,		0.f, 1.f
		};

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


		specter::Shader shader
		{
			{ GL_VERTEX_SHADER, "C:\\Users\\flora\\source\\shaders\\rtx\\quad.glsl.vs" },
			{ GL_FRAGMENT_SHADER, "C:\\Users\\flora\\source\\shaders\\rtx\\quad.glsl.fs" }
		};
		shader.create();
		shader.bind();

		while (!glfwWindowShouldClose(window.getWindow())) {
			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window.getWindow(), GLFW_TRUE);
			}

			glDrawArrays(GL_TRIANGLES, 0, 6);

			glfwSwapBuffers(window.getWindow());
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &quadvao);
		glDeleteBuffers(1, &quadvbo);
		glDeleteTextures(1, &rtx_texture);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	catch (...) {
		std::cout << "Unexpected error somewhere...";
	}
} 