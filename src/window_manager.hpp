#ifndef _WINDOW_MANAGER_HPP_
#define _WINDOW_MANAGER_HPP_

#include <glad/glad.h>
#include <glfw3.h>

#include <memory>

#include "mk_state.hpp"
#include "movement_direction.hpp"
#include "math/vec2.hpp"

namespace specter {

enum class WindowMode_ : uint8_t {
    None,
    Windowed, Borderless, Fullscreen
};

class Cursor;

class WindowManager {

public:

    WindowManager(const vec2u& screen_resolution, const WindowMode_& window_mode, bool DEBUG_MODE = false);
    ~WindowManager();

    GLFWwindow* get_window_pointer() {
        return window;
    }

    MouseKeyboardState get_mk_state() const {
        return mk_state;
    }

    bool get_lmb_pressed() const;
    bool get_rmb_pressed() const;

    float get_mouse_zoom() const;
    vec2i get_mouse_delta() const;
    vec2i get_mouse_position() const;

    MovementDirection get_movement_direction() const;

    vec2u const get_window_resolution() const {
        return screen_resolution;
    }

    void reset_mouse_delta();
    void reset_mouse_zoom();

    void set_title(const char* title);

private:

    GLFWwindow* window = nullptr;
    std::unique_ptr<Cursor> cursor;
    vec2u screen_resolution;

    MouseKeyboardState mk_state;
};

}

#endif 