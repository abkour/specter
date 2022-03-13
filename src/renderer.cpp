#include "renderer.hpp"

namespace specter {

RTX_Renderer::RTX_Renderer(Scene* scene) {
	this->scene = scene;
	frame.resize(scene->camera.resx() * scene->camera.resy());
	
	if (!scene->dynamicFrame) {
		throw std::runtime_error("static frame rendering is not implemented!");
	}

	terminateRendering.store(false);
}

RTX_Renderer::~RTX_Renderer() {
	// In case the thread was not joined in the run() function, join it here.
	if (renderThread.joinable()) {
		renderThread.join();
	}
}

void RTX_Renderer::run() {
	// Open the window using GLFW, initialize GLAD and allow user input via mouse and keyboard.
	window.openWindow(specter::WindowMode::WINDOWED, specter::vec2u(scene->camera.resx(), scene->camera.resy()), "Specter Raytracer");
	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set up the RTX texture
	glGenTextures(1, &image);
	glBindTexture(GL_TEXTURE_2D, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scene->camera.resx(), scene->camera.resy(), 0, GL_RGB, GL_FLOAT, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTextureUnit(0, image);

	//
	//
	// Run the integrator in a seperate thread
	renderThread = std::thread(&RTX_Renderer::runDynamic, this);

	// Quad spanning the entire window
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
	
	// Simple pass-through vertex shader that renders a texture onto a quad
	specter::Shader quadShader =
	{
		{ GL_VERTEX_SHADER, "C:\\Users\\flora\\source\\repos\\specter\\shaders\\quad.glsl.vs" },
		{ GL_FRAGMENT_SHADER, "C:\\Users\\flora\\source\\repos\\specter\\shaders\\quad.glsl.fs" }
	};
	quadShader.create();
	quadShader.bind();

	while (!glfwWindowShouldClose(window.getWindow())) {
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Check if the frame is out-of-date
		std::unique_lock<std::mutex> lck(updateMtx);
		if (updateFrame) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scene->camera.resx(), scene->camera.resy(), 0, GL_RGB, GL_FLOAT, frame.data());
			glGenerateMipmap(GL_TEXTURE_2D);
			updateFrame = false;
		}
		lck.unlock();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();

		// Close this window and terminate the rendering thread.
		if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window.getWindow(), GLFW_TRUE);
			terminateRendering.store(true);
		}
	}

	// join the rendering thread
	if (renderThread.joinable()) {
		renderThread.join();
	}
	
	// Clean up
	glDeleteTextures(1, &image);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void RTX_Renderer::runDynamic() {
	std::cout << "Rendering mesh (parallel)...\n";
	specter::Timer rtxtime;

	// Right now this light is hardcoded until I find a convenient way of 
	// abstracting away light specific rendering
	AmbientLight ambientLight;

	const int nShadowRays = scene->reflection_rays;
	unsigned nSamplesPerDirection = std::sqrt(scene->camera.spp());

	// This buffer holds the cumulative color of the AO integrator.
	std::vector<vec3f> cumulativeColor;
	cumulativeColor.resize(frame.size());

	std::vector<Intersection> intersections;
	intersections.resize(frame.size() * scene->camera.spp());

	std::vector<Ray> rays;
	rays.resize(frame.size() * scene->camera.spp());

	// Optimization
	// Intersections and associated rays need only be computed once, as the camera/scene don't change
	tbb::parallel_for(tbb::blocked_range2d<int>(0, scene->camera.resy(), 0, scene->camera.resx()),
		[&](const tbb::blocked_range2d<int>& r) {
			for (int y = r.rows().begin(); y < r.rows().end(); ++y) {
				for (int x = r.cols().begin(); x < r.cols().end(); ++x) {
					for (int subi = 0; subi < scene->camera.spp(); subi++) {
						std::size_t index = y * scene->camera.resx() * scene->camera.spp();
						index += x * scene->camera.spp() + subi;
						const unsigned spx = x * nSamplesPerDirection + 1;
						const unsigned spy = y * nSamplesPerDirection + 1;
						const unsigned sxoff = subi % nSamplesPerDirection;
						const unsigned syoff = subi / nSamplesPerDirection;

						specter::Ray ray = scene->camera.getRay(specter::vec2u(spx + sxoff, spy + syoff));
						specter::Intersection its;
						intersections[index] = scene->accel.traceRay(ray, its) ? its : Intersection();
						rays[index] = ray;
					}
				}
			}
		}
	);

	// Update the frame every time the AO integrator converges.
	// To accomplish that, we iterate over the entire frame for each shadow ray.
	// At the end of each iteration we communicate to the main thread to update the image texture.
	for (int shadowsCast = 1; shadowsCast < nShadowRays; ++shadowsCast) {
		tbb::parallel_for(tbb::blocked_range2d<int>(0, scene->camera.resy(), 0, scene->camera.resx()),
			[&](const tbb::blocked_range2d<int>& r) {
				for (int y = r.rows().begin(); y < r.rows().end(); ++y) {
					// If window closes rendering should close as well.
					if (terminateRendering.load()) {
						return;
					}
					for (int x = r.cols().begin(); x < r.cols().end(); ++x) {
						specter::vec3f color(0.f);
						// Compute light per subpixel and average out the results
						for (int subi = 0; subi < scene->camera.spp(); subi++) {
							std::size_t index = y * scene->camera.resx() * scene->camera.spp();
							index += x * scene->camera.spp() + subi;

							auto its = intersections[index];
							if (its.isValid()) {
								unsigned normalIndex = scene->mesh.getFace(its.f * 3).n;
								specter::vec3f normal = scene->mesh.getNormal(normalIndex);

								auto& ray = rays[index];
								const specter::vec3f intersectionPoint = ray.o + its.t * ray.d;
								color += ambientLight.sample_light(scene->accel, intersectionPoint, normal);
							}
						}
						const std::size_t index = y * scene->camera.resx() + x;
						cumulativeColor[index] += color / scene->camera.spp();
						frame[index] = cumulativeColor[index] / shadowsCast;
					}
				}
			});

		// Notify the rendering thread, that the contents of the frame buffer need updating.
		std::unique_lock<std::mutex> lck(updateMtx);
		updateFrame = true;
		lck.unlock();
	}
}

}