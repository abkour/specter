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
#include "light.hpp"

static specter::MovementDirection getMovementDirection(GLFWwindow* window);

void renderRasterized(specter::vec3f* vertices, std::size_t nVertices, specter::FaceElement* indices, std::size_t nIndices);

specter::vec3f computeColorFromPointLight(const specter::Light& pointLight, const specter::vec3f& point, const specter::vec3f& surfaceNormal);

int main(int argc, const char** argv) {
	try {
		static const char* filename = "C:\\Users\\flora\\rsc\\assets\\cylinder\\cylinder.obj";
		specter::ObjLoader mesh(filename);
		//renderRasterized(mesh.getVertices(), mesh.getVertexCount(), mesh.getFaces(), mesh.getTriangleCount() * 3);
		
		const specter::Light pointLight(specter::vec3f(10.f, 0.5f, 1.f), specter::vec3f(250'000));
		const specter::vec3f eyepos(2.f, 2.f, -3.f);
		const specter::vec3f eyetarget(0.f, 0.f, 0.f);
		const specter::vec2u screen_resolution(1920, 1080);
		specter::Camera camera(screen_resolution);
		const unsigned nSamplesPerPixel = 1;
		const unsigned nSamplesPerDirection = std::sqrt(nSamplesPerPixel);
		camera.initializeVariables(eyepos, eyetarget, 90.f, nSamplesPerPixel);
		
		std::vector<specter::vec3f> frame;
		frame.resize(specter::product(screen_resolution));

		specter::Accel accel;
		accel.addMesh(&mesh);
		accel.build();

		std::cout << "Rendering mesh...\n";
		specter::Timer rtxtime;

		unsigned nHits = 0;
		for (int y = 0; y < screen_resolution.y; ++y) {
			for (int x = 0; x < screen_resolution.x; ++x) {
				specter::vec3f cumulativeColor(0.f);
				for (int sxoff = 0; sxoff < nSamplesPerDirection; sxoff++) {
					for (int syoff = 0; syoff < nSamplesPerDirection; syoff++) {
						const unsigned spi = syoff * nSamplesPerDirection + sxoff;
						const unsigned spx = x * nSamplesPerDirection + 1;
						const unsigned spy = y * nSamplesPerDirection + 1;
						
						specter::Ray ray = camera.getRay(specter::vec2u(spx + sxoff, spy + syoff));
						specter::Intersection its;

						if (accel.traceRay(ray, its)) {
							unsigned normalIndex = mesh.getFace(its.f * 3).n;
							specter::vec3f normal = mesh.getNormal(normalIndex);

							// Compute exact intersection point using barycentric coordinates
							specter::vec3f bary;
							bary.x = its.uv.x;
							bary.y = its.uv.y;
							bary.z = 1.f - bary.x - bary.y;

							int pixelIndex = sxoff + syoff * nSamplesPerDirection;
							specter::vec3f p0 = mesh.getVertex(mesh.getFace(its.f * 3 + 0).p);
							specter::vec3f p1 = mesh.getVertex(mesh.getFace(its.f * 3 + 1).p);
							specter::vec3f p2 = mesh.getVertex(mesh.getFace(its.f * 3 + 2).p);

							const specter::vec3f intersectionPoint = p0 * bary.x + p1 * bary.y + p2 * bary.z;

							// Cast shadow ray to check for mutual visibility between the surface point and the point light source
							specter::Ray shadowRay;
							shadowRay.d = specter::normalize(pointLight.position - intersectionPoint);
							shadowRay.o = intersectionPoint;// + shadowRay.d * 1e-6;
							shadowRay.invd = 1.f / shadowRay.d;

							specter::Intersection itsShadow;
							if (!accel.traceRay(shadowRay, itsShadow, true)) {
								//cumulativeColor += computeColorFromPointLight(pointLight, intersectionPoint, normal);
								//cumulativeColor += abs(normal);
							}
							cumulativeColor += abs(normal);
							nHits++;
						}
					}
				}
				
				const std::size_t index = y * screen_resolution.x + x;
				cumulativeColor /= static_cast<float>(nSamplesPerPixel);
				frame[index].x = cumulativeColor.x;
				frame[index].y = cumulativeColor.y;
				frame[index].z = cumulativeColor.z;
			}
		}

		std::cout << "Generating image took: " << rtxtime.elapsedTime() << " seconds.\n";
		
		specter::Window window(specter::WindowMode::WINDOWED, screen_resolution, "Specter Raytracer");
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		static float quad[] = 
		{
			// vertices		// uv
			-1.f, -1.f,		0.f, 0.f,
			1.f, -1.f,		1.f, 0.f,
			1.f, 1.f,		1.f, 1.f,

			-1.f, -1.f,		0.f, 0.f,
			1.f, 1.f,		1.f, 1.f,
			-1.f, 1.f,		0.f, 1.f
		};

		GLuint vao, vbo;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));

		specter::Shader quadShader =
		{
			{ GL_VERTEX_SHADER, "C:\\Users\\flora\\source\\shaders\\rtx\\quad.glsl.vs" },
			{ GL_FRAGMENT_SHADER, "C:\\Users\\flora\\source\\shaders\\rtx\\quad.glsl.fs" }
		};
		quadShader.create();
		quadShader.bind();

		GLuint image;
		glGenTextures(1, &image);
		glBindTexture(GL_TEXTURE_2D, image);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_resolution.x, screen_resolution.y, 0, GL_RGB, GL_FLOAT, frame.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTextureUnit(0, image);

		while (!glfwWindowShouldClose(window.getWindow())) {
			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			glfwSwapBuffers(window.getWindow());
			glfwPollEvents();

			if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window.getWindow(), GLFW_TRUE);
			}
		}

		glDeleteTextures(1, &image);
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
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

specter::vec3f computeColorFromPointLight(const specter::Light& pointLight, const specter::vec3f& point, const specter::vec3f& surfaceNormal) {
	const float FourPiSquared = 39.478417604357434;

	specter::vec3f incv = point - pointLight.position;
	specter::vec3f toLight = pointLight.position - point;

	float cosAngle = specter::dot(incv, surfaceNormal / specter::length(incv));

	const float t0 = pointLight.energy.x / FourPiSquared;
	const float t1 = std::max(0.f, cosAngle);
	const float t2 = specter::length(incv) * specter::length(incv);
	const float t3 = t0 * (t1 / t2);

	return specter::vec3f(t3);
}
