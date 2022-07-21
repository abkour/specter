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
	window.enableCursorZoom();
	//window.enableKeyStateCallback();
	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set up the RTX texture
	glGenTextures(1, &image);
	glBindTexture(GL_TEXTURE_2D, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scene->camera.resx(), scene->camera.resy(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	const float borderColor[] = { 0.f, 0.f, 0.f, 0.f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTextureUnit(0, image);

	auto octree = scene->accel.GetOctree();

	std::cout << "Octree statistics\n";
	std::cout << "Max depth: " << octree.GetMaxDepth() << '\n';
	octree.printNodesPerLayer();

	//
	//
	// Run the integrator in a seperate thread
	//renderThread = std::thread(&RTX_Renderer::runDynamic, this);
	renderThread = std::thread(&RTX_Renderer::dev_runDynamic, this);
	//renderThread = std::thread(&RTX_Renderer::dev_runDynamicST, this);


	static float quadvertices[] =
	{
		-1.f, -1.f,
		1.f, -1.f,
		1.f, 1.f,
		-1.f, -1.f,
		1.f, 1.f,
		-1.f, 1.f
	};

	
	static float quadTextureCoordinates[] =
	{
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,

		0.f, 0.f,
		1.f, 1.f,
		0.f, 1.f
	};

	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadvertices) + sizeof(quadTextureCoordinates), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadvertices), quadvertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(quadvertices), sizeof(quadTextureCoordinates), quadTextureCoordinates);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(sizeof(quadvertices)));
	
	// Simple pass-through vertex shader that renders a texture onto a quad
	specter::Shader quadShader =
	{
		{ GL_VERTEX_SHADER, "C:\\Users\\flora\\source\\repos\\specter\\shaders\\quad.glsl.vs" },
		{ GL_FRAGMENT_SHADER, "C:\\Users\\flora\\source\\repos\\specter\\shaders\\quad.glsl.fs" }
	};
	quadShader.create();
	quadShader.bind();
	
	specter::vec2f texCoordBottomLeft(0.f, 0.f);
	specter::vec2 texCoordTopRight(1.f, 1.f);

	float currentCameraZoom = window.getCursorZoom();
	auto zoomFunction = [](float zoomFactor) { return zoomFactor >= 50 ? 0.5f : (zoomFactor < 0.f ) ? 0.f : zoomFactor / 100.f; };

	specter::vec2f pictureMovementDirection(0.f, 0.f);

	float deltatime = 0.f;
	float lasttime = 0.f;

	while (!glfwWindowShouldClose(window.getWindow())) {
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		deltatime = glfwGetTime() - lasttime;
		lasttime += deltatime;

		if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
			pictureMovementDirection.y = 1.f;
		}
		if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
			pictureMovementDirection.y = -1.f;
		}
		if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
			pictureMovementDirection.x = -1.f;
		}
		if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
			pictureMovementDirection.x = 1.f;
		}

		// If keys are released we need to also reset movement direction (velocity)
		if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_RELEASE) {
			pictureMovementDirection.y = 0.f;
		}
		if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_RELEASE) {
			pictureMovementDirection.x = 0.f;
		}


		if (currentCameraZoom != window.getCursorZoom() || pictureMovementDirection != specter::vec2f(0.f, 0.f)) {
			currentCameraZoom = window.getCursorZoom();
			const float zoomScale = zoomFunction(currentCameraZoom);
			
			const float sFrameTime = deltatime > 0.1f ? 0.1f : deltatime;

			texCoordBottomLeft.x += pictureMovementDirection.x * (0.01f * zoomScale);
			texCoordBottomLeft.y += pictureMovementDirection.y * (0.01f * zoomScale);
			texCoordTopRight.x += pictureMovementDirection.x * (0.01f * zoomScale);
			texCoordTopRight.y += pictureMovementDirection.y * (0.01f * zoomScale);

			// Adjust for the zoom factor
			float newTextureCoordinates[] =
			{
				texCoordBottomLeft.x + zoomScale, texCoordBottomLeft.y + zoomScale,
				texCoordTopRight.x - zoomScale, texCoordBottomLeft.y + zoomScale,
				texCoordTopRight.x - zoomScale, texCoordTopRight.y - zoomScale,

				texCoordBottomLeft.x + zoomScale, texCoordBottomLeft.y + zoomScale,
				texCoordTopRight.x - zoomScale, texCoordTopRight.y - zoomScale,
				texCoordBottomLeft.x + zoomScale, texCoordTopRight.y - zoomScale
			};

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(quadvertices), sizeof(quadTextureCoordinates), newTextureCoordinates);
		}

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

						specter::Ray ray = scene->camera.getRay(specter::vec2f(spx + sxoff, spy + syoff));
						specter::Intersection its;
						intersections[index] = scene->accel.traceRay(ray, its) ? its : Intersection();
						rays[index] = ray;
					}
				}
			}
		}
	);

	// Update the frame every time the integrator converges.
	// To accomplish that, we iterate over the entire frame for each shadow ray.
	// At the end of each iteration we communicate to the main thread to update the image texture.
	for (int shadowsCast = 1; shadowsCast < nShadowRays; ++shadowsCast) {
		tbb::parallel_for(tbb::blocked_range2d<int>(0, scene->camera.resy(), 0, scene->camera.resx()),
			[&](const tbb::blocked_range2d<int>& r) {
				for (int y = r.rows().begin(); y < r.rows().end(); ++y) {
					// If window closes rendering should stop as well.
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
								unsigned normalIndex = scene->model->GetFace(its.f * 3).n;
								specter::vec3f normal = scene->model->GetNormal(normalIndex);

								auto& ray = rays[index];
								const specter::vec3f intersectionPoint = ray.o + its.t * ray.d;
								//color += scene->light->sample_light(scene->accel, intersectionPoint, normal);
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

vec3f RTX_Renderer::dev_pixel_color(const Ray& ray, int reflectionDepth) {
	if (reflectionDepth <= 0) {
		return vec3f(0.f);
	}

	Intersection its;
	if (!scene->accel.traceRay(ray, its)) {
		return vec3f(0.f);
	} 

	Ray scattered;
	vec3f attenuation;
	vec3f emitted = its.mat_ptr->emitted(its.u, its.v, its.p);
	if (!its.mat_ptr->scatter(ray, its, scattered, attenuation)) {
		return emitted;
	}

	return emitted + attenuation * dev_pixel_color(scattered, reflectionDepth - 1);
}

void RTX_Renderer::dev_runDynamicST() {
	std::cout << "[DEV] Rendering mesh (scalar)...\n";
	
	Timer timer;

	unsigned int spp = 32;
	unsigned reflectionDepth = 16;
	for (int y = 0; y < scene->camera.resy(); ++y) {
		if (terminateRendering.load()) return;
		for (int x = 0; x < scene->camera.resx(); ++x) {
			vec3f color(0.f);
			for (int k = 0; k < spp; ++k) {
				vec2f off = RandomEngine::get_random_float();
				specter::Ray ray = scene->camera.getRay(specter::vec2f(x + off.x, y + off.y));
				if (x >= 700 && x <= 900 && y >= 500 && y <= 700) {
					color = vec3f(1.f);
				} else {
					color += dev_pixel_color(ray, reflectionDepth);
				}
			}
			const std::size_t index = y * scene->camera.resx() + x;
			//frame[index] = color / (float)spp;
			frame[index].x = std::sqrt(color.x / (float)spp);
			frame[index].y = std::sqrt(color.y / (float)spp);
			frame[index].z = std::sqrt(color.z / (float)spp);
		}
	}

	std::cout << "[DEV] Finshed rendering!\n";
	std::cout << "[DEV] Elapsed time: " << timer.elapsedTime() << '\n';

	// Notify the rendering thread, that the contents of the frame buffer need updating.
	std::unique_lock<std::mutex> lck(updateMtx);
	updateFrame = true;
	lck.unlock();
}

void RTX_Renderer::dev_runDynamic() {
	std::cout << "[DEV] Rendering mesh (parallel)...\n";

	Timer timer;

	unsigned int spp = scene->camera.spp();
	unsigned reflectionDepth = 16;
	tbb::parallel_for(tbb::blocked_range2d<int>(0, scene->camera.resy(), 0, scene->camera.resx()),
		[&](tbb::blocked_range2d<int> r) {
			for (int y = r.rows().begin(); y < r.rows().end(); ++y) {
				if (terminateRendering.load()) return;
				for (int x = r.cols().begin(); x < r.cols().end(); ++x) {
					vec3f color(0.f);
					for (int k = 0; k < spp; ++k) {
						vec2f off = RandomEngine::get_random_float();
						specter::Ray ray = scene->camera.getRay(specter::vec2f(x + off.x, y + off.y));
						color += dev_pixel_color(ray, reflectionDepth);
					}
					const std::size_t index = y * scene->camera.resx() + x;
					frame[index].x = std::sqrt(color.x / (float)spp);
					frame[index].y = std::sqrt(color.y / (float)spp);
					frame[index].z = std::sqrt(color.z / (float)spp);
				}
			}
		});

	std::cout << "[DEV] Finshed rendering!\n";
	std::cout << "[DEV] Elapsed time: " << timer.elapsedTime() << '\n';

	// Notify the rendering thread, that the contents of the frame buffer need updating.
	std::unique_lock<std::mutex> lck(updateMtx);
	updateFrame = true;
	lck.unlock();
}

}