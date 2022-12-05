#pragma warning(disable:4996)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"

#include "imgui.h"
#include "backends\imgui_impl_glfw.h"
#include "backends\imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION

#include "misc.hpp"
#include "view.hpp"
#include "renderer.hpp"
#include "raster/raster_engine.hpp"
#include "rtx/cpu_lbvh_helpers.hpp"
#include "rtx/cpu_lbvh.hpp"
#include "rtx/filters.hpp"

void renderRasterized(const char* filename);
void renderRTX(const char* scene_descriptor_file);
void testFilters();
void test_cpu_lbvh_implementation(const char* filename);
void test_imgui(const char* scene_descriptor_file);

int main(int argc, const char** argv) {
	try {
		std::cout << "SPECTER 3D RENDERING ENGINE\n\n";
		//test_imgui(argv[1]);
		//renderRTX(argv[1]);
		renderRasterized(argv[1]);
		//test_cpu_lbvh_implementation(argv[1]);
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what();
	}
	catch (...) {
		std::cout << "Unexpected error somewhere...";
	}
}

void test_imgui(const char* scene_descriptor_file) {
	std::cout << "Hello, IMGUI\n";
	const char* glsl_version = "#version 450";

	specter::SceneDescriptor scene_descriptor(scene_descriptor_file);
	specter::Scene scene(scene_descriptor);
	
	const specter::vec2u screen_resolution(1920, 1080);
	specter::Window window_manager(specter::WindowMode::WINDOWED, screen_resolution, "specter (raster mode)");
	glfwSetInputMode(window_manager.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	window_manager.enableCursorCallback();
	glfwSwapInterval(0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	auto indices = scene.model->GetFaces();
	auto vertices = scene.model->GetVertices();
	auto nIndices = scene.model->GetFaceCount();
	auto nVertices = scene.model->GetVertexCount();
	
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
	glNamedBufferData(vbo, nVertices * sizeof(specter::vec3f), NULL, GL_STATIC_DRAW);
	glNamedBufferSubData(vbo, 0, nVertices * sizeof(specter::vec3f), vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned), vertexIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(specter::vec3f), 0);

	specter::Shader shader
	{
		{ GL_VERTEX_SHADER, ROOT_DIRECTORY + std::string("\\src\\shaders\\simple.glsl.vs") },
		{ GL_FRAGMENT_SHADER, ROOT_DIRECTORY + std::string("\\src\\shaders\\simple.glsl.fs") }
	};
	shader.create();
	shader.bind();

	specter::vec3f eyePos(0.f, 0.f, -2.f);
	specter::vec3f eyeDir(0.f);
	specter::View view(eyePos, eyeDir);

	specter::mat4f proj = specter::perspective(specter::radians(45.f), 1920.f / 1080.f, 0.1, 3000.f);

	float deltatime = 0.f;
	float lasttime = 0.f;

	bool key_c_hit = false;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

	auto window = window_manager.getWindow();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

	bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while(!glfwWindowShouldClose(window)) {
		        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
	}
}

void test_cpu_lbvh_implementation(const char* filename) {
	std::cout << "Testing CPU_LBVH implementation!\n";
	specter::SceneDescriptor scene_descriptor(filename);
	specter::Scene scene(scene_descriptor);
	specter::CPU_LBVH lbvh;
	lbvh.build(scene.model);
	std::cout << "Testing BVH validity...\n";
	bool bvh_valid = lbvh.isValid();
	std::cout << "\nBVH: " << (bvh_valid ? "Valid" : "Invalid") << '\n';
}

void renderRTX(const char* scene_descriptor_file) {
	specter::SceneDescriptor scene_descriptor(scene_descriptor_file);
	specter::Scene scene(scene_descriptor);
	specter::RTX_Renderer renderer(scene);
	renderer.run();
}

void renderRasterized(const char* scene_descriptor_file) {
	specter::SceneDescriptor scene_descriptor(scene_descriptor_file);
	specter::Scene scene(scene_descriptor);
	specter::RasterRenderer renderer(scene);
	renderer.run();
}

void testFilters() {
	specter::vec2u screen_resolution(1920, 1080);
	specter::Window window(specter::WindowMode::WINDOWED, screen_resolution, "Filter test");

	int width, height, nrChannels;
	unsigned char* data = stbi_load("C:\\Users\\flora\\source\\repos\\specter\\results\\dev\\random.png", &width, &height, &nrChannels, 4);
	if (data) {
		unsigned char* output_image = new unsigned char[width * height * 4];
		specter::Timer filtertime;
		specter::grayscale_filter(output_image, data, specter::vec2u(width, height));
		std::cout << "FIltering took: " << filtertime.elapsedTime() << '\n';
		stbi_write_png(	"C:\\Users\\flora\\source\\repos\\specter\\results\\dev\\gray_random.png", 
						width, 
						height,
						4,
						output_image, 
						width * 4);
		delete[] output_image;
	} else {
		std::cout << "Failed reading data!\n";
	}
	stbi_image_free(data);
}