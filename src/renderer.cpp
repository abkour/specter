#include "renderer.hpp"

namespace specter {

// This constructor is strongly coupled to the scene descriptor. Maybe transfer the initialization
// of light objects to the Scene object?
RTX_Renderer::RTX_Renderer(Scene* scene) {
	this->scene = scene;
	frame.resize(product(scene->camera.getResolution()));
}

RTX_Renderer::~RTX_Renderer() {}

void RTX_Renderer::run() {
	std::cout << "Rendering mesh (parallel)...\n";
	
	specter::Timer rtxtime;

	const int nShadowRays = 32;
	AmbientLight ambientLight;
	unsigned nSamplesPerDirection = std::sqrt(1);
	tbb::parallel_for(tbb::blocked_range2d<int>(0, scene->camera.getResolution().y, 0, scene->camera.getResolution().x),
		[&](const tbb::blocked_range2d<int>& r) {
			for (int y = r.rows().begin(); y < r.rows().end(); ++y) {
				for (int x = r.cols().begin(); x < r.cols().end(); ++x) {
					specter::vec3f cumulativeColor(0.f);
					for (int sxoff = 0; sxoff < nSamplesPerDirection; sxoff++) {
						for (int syoff = 0; syoff < nSamplesPerDirection; syoff++) {
							const unsigned spi = syoff * nSamplesPerDirection + sxoff;
							const unsigned spx = x * nSamplesPerDirection + 1;
							const unsigned spy = y * nSamplesPerDirection + 1;

							specter::Ray ray = scene->camera.getRay(specter::vec2u(spx + sxoff, spy + syoff));
							specter::Intersection its;

							if (scene->accel.traceRay(ray, its)) {

								unsigned normalIndex = scene->mesh.getFace(its.f * 3).n;
								specter::vec3f normal = scene->mesh.getNormal(normalIndex);

								if (debugMode == SPECTER_DEBUG_DISPLAY_NORMALS) {
									cumulativeColor += abs(normal);
								}
								else {
									const specter::vec3f intersectionPoint = ray.o + its.t * ray.d;
									for (int i = 0; i < nShadowRays; ++i) {
										cumulativeColor += ambientLight.sample_light(scene->accel, intersectionPoint, normal);
									}
									/*
									unsigned normalIndex = mesh.getFace(its.f * 3).n;
									specter::vec3f normal = mesh.getNormal(normalIndex);

									const specter::vec3f intersectionPoint = ray.o + its.t * ray.d;

									// Cast shadow ray to check for mutual visibility between the surface point and the point light source
									specter::Ray shadowRay;
									shadowRay.d = specter::normalize(light->position - intersectionPoint);
									shadowRay.o = intersectionPoint + normal * 1e-5;	// Avoid self-shadowing
									shadowRay.invd = 1.f / shadowRay.d;

									specter::Intersection itsShadow;
									if (!accel.traceRay(shadowRay, itsShadow, true)) {
										cumulativeColor += light->sample_Light(intersectionPoint, normal);
									}*/
								}
							}
						}
					}
					cumulativeColor /= nShadowRays;
					const std::size_t index = y * scene->camera.getResolution().x + x;
					frame[index] = cumulativeColor;
				}
			}
		}
	);

	std::cout << "Generating image took: " << rtxtime.elapsedTime() << " seconds.\n";
	scene->accel.dbg_print();

	window.openWindow(specter::WindowMode::WINDOWED, specter::vec2u(scene->camera.getResolution().x, scene->camera.getResolution().y), "Specter Raytracer");
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scene->camera.getResolution().x, scene->camera.getResolution().y, 0, GL_RGB, GL_FLOAT, frame.data());
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

}