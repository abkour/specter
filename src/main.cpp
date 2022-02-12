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

static specter::MovementDirection getMovementDirection(GLFWwindow* window);

int main(int argc, const char** argv) {
	try {
		static const char* filename = "C:\\Users\\flora\\rsc\\assets\\cube\\cube.obj";
		specter::ObjLoader mesh(filename);
		const specter::vec3f eyepos(2.f, 2.f, 3.f);
		const specter::vec3f eyetarget(0.f, 0.f, 0.f);
		const specter::vec2u screen_resolution(728, 728);
		specter::Camera camera(screen_resolution);
		const unsigned nSamplesPerPixel = 4;
		const unsigned nSamplesPerDirection = std::sqrt(nSamplesPerPixel);
		camera.initializeVariables(eyepos, eyetarget, 90.f, nSamplesPerPixel);
		
		std::vector<specter::vec3f> frame;
		frame.resize(specter::product(screen_resolution));

		std::cout << "Building octree of mesh...\n";
		specter::Timer octreeTimer;
		specter::Node* root = new specter::Node;
		root->nIndices = mesh.getTriangleCount();
		root->bbox = mesh.computeBoundingBox();
		root->m_children = nullptr;
		root->indices = nullptr;

		std::vector<uint32_t> initialIndexList;
		initialIndexList.resize(mesh.getTriangleCount() * 3);
		for (int i = 0; i < initialIndexList.size(); ++i) {
			initialIndexList[i] = i;
		}

		specter::buildOctree(root, mesh.getVertices(), mesh.getFaces(), initialIndexList.data());
		std::cout << "Construction finished in: " << octreeTimer.elapsedTime() << " seconds.\n";

		specter::freeOctree(root);

		std::cout << "Rendering mesh...\n";
		specter::Timer rtxtime;

		for (int y = 0; y < screen_resolution.y; ++y) {
			for (int x = 0; x < screen_resolution.x; ++x) {
				unsigned fs[nSamplesPerPixel];
				std::fill_n(fs, nSamplesPerPixel, std::numeric_limits<unsigned>::max());
				for (int px = 0; px < nSamplesPerDirection; px++) {
					for (int py = 0; py < nSamplesPerDirection; py++) {
						const unsigned spi = py * nSamplesPerDirection + px;
						const unsigned spx = x * nSamplesPerDirection + 1;
						const unsigned spy = y * nSamplesPerDirection + 1;
						const specter::Ray ray = camera.getRay(specter::vec2u(spx + px, spy + py));
						
						float mint = std::numeric_limits<float>::max();
						
						for (int i = 0; i < mesh.getTriangleCount(); ++i) {
							float u, v, t;
							if (mesh.rayIntersectionV2(ray, i, u, v, t)) {
								if (t < mint && t > 0.f) {
									mint = t;
									fs[spi] = i;
								}
							}
						}
					}
				}

				specter::vec3f cumulativeColor(0.f);

				for (int i = 0; i < nSamplesPerPixel; ++i) {
					if (fs[i] != std::numeric_limits<unsigned>::max()) {
						unsigned normalIndex = mesh.getFace(fs[i]).z;
						specter::vec3f normal = mesh.getNormal(normalIndex);
						cumulativeColor += abs(normal);
					} 
				}

				const std::size_t index = y * screen_resolution.x + x;
				cumulativeColor /= static_cast<float>(nSamplesPerPixel);
				frame[index] = cumulativeColor;
			}
		}

		std::cout << "Generating image took: " << rtxtime.elapsedTime() << " seconds.\n";

		specter::Window window(specter::WindowMode::WINDOWED, screen_resolution, "Specter Raytracer");
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		window.enableCursorCallback();
		glfwSwapInterval(0);
		glEnable(GL_DEPTH_TEST);

		specter::Shader shader
		{
			{ GL_VERTEX_SHADER, "C:\\Users\\flora\\source\\shaders\\rtx\\quad.glsl.vs" },
			{ GL_FRAGMENT_SHADER, "C:\\Users\\flora\\source\\shaders\\rtx\\quad.glsl.fs" }
		};
		shader.create();
		shader.bind();

		GLuint rtx_texture;
		glGenTextures(1, &rtx_texture);
		glBindTexture(GL_TEXTURE_2D, rtx_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_resolution.x, screen_resolution.y, 0, GL_RGB, GL_FLOAT, frame.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTextureUnit(0, rtx_texture);

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
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
		
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

		//delete root;
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