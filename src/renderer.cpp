#include "renderer.hpp"

namespace specter {

RTX_Renderer::RTX_Renderer(Scene& pScene) {
	this->scene = std::make_shared<Scene>(pScene);
	frame.resize(scene->camera.resx() * scene->camera.resy());
	
	if (!scene->dynamicFrame) {
		throw std::runtime_error("static frame rendering is not implemented!");
	}

	// Integrator
	integrator = scene->integrator;

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
	window.openWindow(specter::WindowMode::WINDOWED, specter::vec2u(scene->camera.resx(), scene->camera.resy()), "specter (pathtracer)");
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

	//
	//
	// Run the integrator in a seperate thread
	renderThread = std::thread(&RTX_Renderer::runDynamic, this);

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
	std::cout << "[DEV] Rendering mesh (parallel)...\n";

	std::vector<vec3f> cumulativeColor;
	cumulativeColor.resize(frame.size());
	
	bool MAIN_FORCED_EXIT = false;
	Timer timer;
	int k = 0;

	// This process is creating k frames. These frames are stored as an amalgamation in the 
	// cumulativeColor vector. Before processing the next frame, we average out the currently
	// available frames and signal to the main thread that the frame vector is ready for updating.
	// After all k frames have been rendered, we terminate this thread, after providing some 
	// performance metrics.
	for (; k < scene->spp; ++k) {
		tbb::parallel_for(tbb::blocked_range2d<int>(0, scene->camera.resy(), 0, scene->camera.resx()),
			[&](tbb::blocked_range2d<int> r) {
				for (int y = r.rows().begin(); y < r.rows().end(); ++y) {
					// We received a signal from the main thread to terminate the rendering process.
					if (terminateRendering.load()) {
						MAIN_FORCED_EXIT = true;
						break;
					}
					for (int x = r.cols().begin(); x < r.cols().end(); ++x) {
						vec2f off = RandomEngine::get_random_float();
						specter::Ray ray = scene->camera.getRay(specter::vec2f(x + off.x, y + off.y));
						
						const std::size_t index = y * scene->camera.resx() + x;
						cumulativeColor[index] += integrator->render(*scene->accel.get(), ray);
						
						frame[index].x = std::sqrt(cumulativeColor[index].x / ((float)k + 1.f));
						frame[index].y = std::sqrt(cumulativeColor[index].y / ((float)k + 1.f));
						frame[index].z = std::sqrt(cumulativeColor[index].z / ((float)k + 1.f));
					}
				}
			});
		if (MAIN_FORCED_EXIT) {
			break;
		}
		// Notify the rendering thread, that the contents of the frame buffer need updating.
		std::unique_lock<std::mutex> lck(updateMtx);
		updateFrame = true;
	}

	// Before terminating, tell the user how many pixels have been rendered in this frame.
	if (MAIN_FORCED_EXIT) {
		int nPixelsRendered = 0;
		for (auto& c : cumulativeColor) {
			if (c != vec3f(0.f)) {
				nPixelsRendered++;
			}
		}
		std::cout << "Premature termination.\nIn this frame " << nPixelsRendered
			<< "/" << scene->camera.resx() * scene->camera.resy() << " pixels have been rendered\n";
	}
	
	auto elapsed_time = timer.elapsedTime();
	std::cout << "[DEV] Finshed rendering!\n";
	std::cout << "[DEV] Spp computed " << k << "/" << scene->spp << "\n";
	std::cout << "[DEV] Elapsed time: " << elapsed_time << '\n';
	std::cout << "[Dev] Average frame time: " << elapsed_time / k << "\n";
}

}