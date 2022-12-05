#pragma once

#include "imgui.h"
#include "backends\imgui_impl_glfw.h"
#include "backends\imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION

namespace specter {

class ImguiWrapper {

public:

    ImguiWrapper(GLFWwindow* window);
    ~ImguiWrapper();

    void update();
    void render(GLFWwindow* window);

protected:

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool show_demo_window = false;
    bool show_another_window = false;
};

}