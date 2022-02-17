#include "renderer.hpp"


namespace specter {

RTX_Renderer::RTX_Renderer(const Scene& scene) {
	std::cout << scene << '\n';
	
	mesh.open_read(scene.meshPath.c_str());
	
	accel.addMesh(&mesh);
	accel.build();

	camera.setResolution(scene.screenResolution);
	camera.initializeVariables(scene.cameraPosition, scene.cameraTarget, scene.cameraFov, scene.samplesPerPixel);
}

void RTX_Renderer::run() {

	const specter::PointLight pointLight(specter::vec3f(-70, 3.f, 1.f), specter::vec3f(50'000));

	std::vector<specter::vec3f> frame;
	frame.resize(camera.getResolution().x * camera.getResolution().y);

	std::cout << "Rendering mesh...\n";
	specter::Timer rtxtime;

	unsigned nSamplesPerDirection = 1;
	unsigned nHits = 0;
	for (int y = 0; y < camera.getResolution().y; ++y) {
		for (int x = 0; x < camera.getResolution().x; ++x) {
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

						const specter::vec3f intersectionPoint = ray.o + its.t * ray.d;

						// Cast shadow ray to check for mutual visibility between the surface point and the point light source
						specter::Ray shadowRay;
						shadowRay.d = specter::normalize(pointLight.position - intersectionPoint);
						shadowRay.o = intersectionPoint + normal * 1e-5;	// Avoid self-shadowing
						shadowRay.invd = 1.f / shadowRay.d;

						specter::Intersection itsShadow;
						if (!accel.traceRay(shadowRay, itsShadow, true)) {
							cumulativeColor += pointLight.sample_Light(intersectionPoint, normal);
						}
					}
				}
			}

			const std::size_t index = y * camera.getResolution().x + x;
			cumulativeColor /= static_cast<float>(1);
			frame[index].x = cumulativeColor.x;
			frame[index].y = cumulativeColor.y;
			frame[index].z = cumulativeColor.z;
		}
	}

	std::cout << "Generating image took: " << rtxtime.elapsedTime() << " seconds.\n";

	specter::Window window(specter::WindowMode::WINDOWED, specter::vec2u(camera.getResolution().x, camera.getResolution().y), "Specter Raytracer");
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera.getResolution().x, camera.getResolution().y, 0, GL_RGB, GL_FLOAT, frame.data());
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