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

void renderRasterized(specter::vec3f* vertices, std::size_t nVertices, specter::FaceElement* indices, std::size_t nIndices);

int main(int argc, const char** argv) {
	try {
		static const char* filename = "C:\\Users\\flora\\rsc\\assets\\cube\\cube.obj";
		//static const char* filename = "C:\\Users\\flora\\rsc\\assets\\monkey\\monkey.obj";
		//static const char* filename = "C:\\Users\\flora\\rsc\\assets\\bunny\\bunny.obj";
		specter::ObjLoader mesh(filename);
		
		renderRasterized(mesh.getVertices(), mesh.getVertexCount(), mesh.getFaces(), mesh.getTriangleCount() * 3);

		return 0;

		/*
		const specter::vec3f eyepos(2.f, 2.f, 3.f);
		const specter::vec3f eyetarget(0.f, 0.f, 0.f);
		const specter::vec2u screen_resolution(728, 728);
		specter::Camera camera(screen_resolution);
		const unsigned nSamplesPerPixel = 1;
		const unsigned nSamplesPerDirection = std::sqrt(nSamplesPerPixel);
		camera.initializeVariables(eyepos, eyetarget, 90.f, nSamplesPerPixel);*/
		/*
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
		initialIndexList.resize(mesh.getTriangleCount());
		for (int i = 0; i < initialIndexList.size(); ++i) {
			initialIndexList[i] = i;
		}

		specter::buildOctree(root, mesh.getVertices(), mesh.getFaces(), initialIndexList.data());
		std::cout << "Construction finished in: " << octreeTimer.elapsedTime() << " seconds.\n";
		
		specter::printOctreeNumbers();

		std::cout << "Rendering mesh...\n";
		specter::Timer rtxtime;

		unsigned nHits = 0;

		for (int y = 0; y < screen_resolution.y; ++y) {
			for (int x = 0; x < screen_resolution.x; ++x) {
				unsigned fs[nSamplesPerPixel];
				std::fill_n(fs, nSamplesPerPixel, std::numeric_limits<unsigned>::max());
				for (int sxoff = 0; sxoff < nSamplesPerDirection; sxoff++) {
					for (int syoff = 0; syoff < nSamplesPerDirection; syoff++) {
						const unsigned spi = syoff * nSamplesPerDirection + sxoff;
						const unsigned spx = x * nSamplesPerDirection + 1;
						const unsigned spy = y * nSamplesPerDirection + 1;
						const specter::Ray ray = camera.getRay(specter::vec2u(spx + sxoff, spy + syoff));
						
						/*
						unsigned f = std::numeric_limits<unsigned>::max();
						float u, v, t = std::numeric_limits<float>::max();
						specter::rayTraversal(&mesh, root, ray, u, v, t, f);

						if (f != std::numeric_limits<unsigned>::max()) {
							fs[spi] = f;
						}
						*/
						/*
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
		std::cout << "Number of rays hit: " << nHits << '\n';
		*/
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

void renderRasterized(specter::vec3f* vertices, std::size_t nVertices, specter::FaceElement* indices, std::size_t nIndices) {
	specter::Window window(specter::WindowMode::WINDOWED, specter::vec2u(728), "Specter Raytracer");
	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	window.enableCursorCallback();
	glfwSwapInterval(0);
	glEnable(GL_DEPTH_TEST);

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

	while (!glfwWindowShouldClose(window.getWindow())) {
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		deltatime = glfwGetTime() - lasttime;
		lasttime += deltatime;

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
		glUniformMatrix4fv(glGetUniformLocation(shader.id(), "view"), 1, GL_FALSE, &view.view[0][0]);

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