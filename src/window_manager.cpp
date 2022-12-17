#include "window_manager.hpp"
#include <stdexcept>

namespace specter {

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

WindowManager::WindowManager(
                const vec2u& screen_resolution, 
                const WindowMode_& window_mode,
                bool DEBUG_MODE)
{
    this->screen_resolution = screen_resolution;
    
    if (!glfwInit()) {
        throw std::runtime_error("[Class::WindowManager] GLFW could not be initialized!");
	}

    if(DEBUG_MODE) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);

    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    if(!primary_monitor) {
        throw std::runtime_error("[Class::WindowManager] Couldn't identify primary monitor!");
    }

    GLFWvidmode* video_mode = const_cast<GLFWvidmode*>(glfwGetVideoMode(primary_monitor)); 
    if(!video_mode) {
        throw std::runtime_error("[Class::WindowManager] Couldn't identify video mode of primary monitor!");
    }

    switch(window_mode) {
    case WindowMode_::Windowed:
        window = glfwCreateWindow(screen_resolution.x, screen_resolution.y, "Title", NULL, NULL);
        break;
    case WindowMode_::Borderless:
        glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);
        window = glfwCreateWindow(screen_resolution.x, screen_resolution.y, "Title", primary_monitor, NULL);
        break;
    case WindowMode_::Fullscreen:
        window = glfwCreateWindow(screen_resolution.x, screen_resolution.y, "Title", primary_monitor, NULL);
        break;
    default:
        throw std::runtime_error("[Class::WindowManager] No window mode specified!");
        break;
    }

	if (!window) {
        throw std::runtime_error("[Class::WindowManager] Window could not be created!");
	}

	glfwMakeContextCurrent(window);
	if (glfwGetError(NULL)) {
        throw std::runtime_error("[Class::WindowManager] Window could not be made current!");
	}
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("[Class::WindowManager] GLAD could not be loaded!");
	}

    if(DEBUG_MODE) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
    }
    
    //cursor = std::make_unique<Cursor>(screen_resolution.x / 2, screen_resolution.y / 2);
    
    std::memset(&mk_state, 0, sizeof(MouseKeyboardState));
    mk_state.initialEntry = true;
    mk_state.pos.x = screen_resolution.x / 2;
    mk_state.pos.y = screen_resolution.y / 2;

    glfwSetWindowUserPointer(window, &mk_state);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallback);
}

WindowManager::~WindowManager() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// Cursor definition
class Cursor {

public:

    Cursor(float xpos, float ypos) 
        : lmb_pressed(false)
        , rmb_pressed(false)
        , zoom(0.f)
        , pos(xpos, ypos)
        , delta(0.f, 0.f)
        , initialEntry(true)
    {}

    bool lmb_pressed, rmb_pressed;

    float zoom;
    vec2i pos;
    vec2i delta;
    bool initialEntry;
};


void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    const char* source_str = NULL;
    switch(source) {
    case GL_DEBUG_SOURCE_API:
        source_str = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        source_str = "WINDOW_SYSTEM";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        source_str = "SHADER_COMPILER";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        source_str = "THIRD_PARTY";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        source_str = "APPLICATION";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        source_str = "OTHER";
        break;
    default:
        break;
    }

    const char* type_str = NULL;
    switch(type) {
    case GL_DEBUG_TYPE_ERROR:
        type_str = "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type_str = "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type_str = "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        type_str = "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        type_str = "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_MARKER:
        type_str = "MARKER";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        type_str = "PUSH_GROUP";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        type_str = "POP_GROUP";
        break;
    case GL_DEBUG_TYPE_OTHER:
        type_str = "OTHER";
        break;
    default:
        break;
    }

    const char* severity_str = NULL;
    switch(severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        severity_str = "HIGH";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        severity_str = "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        severity_str = "LOW";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        severity_str = "NOTIFICATION";
        break;
    default:
        break;
    }

  fprintf( stderr, "GL CALLBACK: %s source = %s, type = %s, severity = %s, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            source_str, type_str, severity_str, message );
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    MouseKeyboardState* cursor = 
        reinterpret_cast<MouseKeyboardState*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        cursor->LMB = true;
    } else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        cursor->LMB = false;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        cursor->RMB = true;
    } else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        cursor->RMB = false;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    MouseKeyboardState* mk_state =
        reinterpret_cast<MouseKeyboardState*>(glfwGetWindowUserPointer(window));
    
    mk_state->zoom = yoffset;
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    MouseKeyboardState* mk_state =
        reinterpret_cast<MouseKeyboardState*>(glfwGetWindowUserPointer(window));

    if(mk_state->initialEntry) {
        mk_state->pos.x = xpos;
        mk_state->pos.y = ypos;
        mk_state->initialEntry = false;
    }

    mk_state->delta.x = xpos - mk_state->pos.x;
    mk_state->delta.y = mk_state->pos.y - ypos;
    mk_state->pos.x = xpos;
    mk_state->pos.y = ypos;
}

float WindowManager::get_mouse_zoom() const {
    return mk_state.zoom;
}

vec2i WindowManager::get_mouse_delta() const {
    return mk_state.delta;
}

vec2i WindowManager::get_mouse_position() const {
    return mk_state.pos;
}

bool WindowManager::get_lmb_pressed() const {
    return mk_state.LMB;
}

bool WindowManager::get_rmb_pressed() const {
    return mk_state.RMB;
}

void WindowManager::reset_mouse_zoom() {
    mk_state.zoom = 0.f;
}

void WindowManager::reset_mouse_delta() { 
    mk_state.delta = vec2i(0.f);
}

void WindowManager::set_title(const char* title) {
    glfwSetWindowTitle(window, title);
}

// Prototype definitions
MovementDirection WindowManager::get_movement_direction() const {
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        return MovementDirection::Forward;
    } else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        return MovementDirection::Backward;
    } else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        return MovementDirection::Left;
    } else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        return MovementDirection::Right;
    }
    return MovementDirection::None;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    MouseKeyboardState* state =
        reinterpret_cast<MouseKeyboardState*>(glfwGetWindowUserPointer(window));
    
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    switch (key) {
    // Alpha-numerical
    case GLFW_KEY_0:
        state->Num_0 = action;
        break;
    case GLFW_KEY_1:
        state->Num_1 = action;
        break;
    case GLFW_KEY_2:
        state->Num_2 = action;
        break;
    case GLFW_KEY_3:
        state->Num_3 = action;
        break;
    case GLFW_KEY_4:
        state->Num_4 = action;
        break;
    case GLFW_KEY_5:
        state->Num_5 = action;
        break;
    case GLFW_KEY_6:
        state->Num_6 = action;
        break;
    case GLFW_KEY_7:
        state->Num_7 = action;
        break;
    case GLFW_KEY_8:
        state->Num_8 = action;
        break;
    case GLFW_KEY_9:
        state->Num_9 = action;
        break;
    case GLFW_KEY_A:
        state->A = action;
        break;
    case GLFW_KEY_B:
        state->B = action;
        break;
    case GLFW_KEY_C:
        state->C = action;
        break;
    case GLFW_KEY_D:
        state->D = action;
        break;
    case GLFW_KEY_E:
        state->E = action;
        break;
    case GLFW_KEY_F:
        state->F = action;
        break;
    case GLFW_KEY_G:
        state->G = action;
        break;
    case GLFW_KEY_H:
        state->H = action;
        break;
    case GLFW_KEY_I:
        state->I = action;
        break;
    case GLFW_KEY_J:
        state->J = action;
        break;
    case GLFW_KEY_K:
        state->K = action;
        break;
    case GLFW_KEY_L:
        state->L = action;
        break;
    case GLFW_KEY_M:
        state->M = action;
        break;
    case GLFW_KEY_N:
        state->N = action;
        break;
    case GLFW_KEY_O:
        state->O = action;
        break;
    case GLFW_KEY_P:
        state->P = action;
        break;
    case GLFW_KEY_Q:
        state->Q = action;
        break;
    case GLFW_KEY_R:
        state->R = action;
        break;
    case GLFW_KEY_S:
        state->S = action;
        break;
    case GLFW_KEY_T:
        state->T = action;
        break;
    case GLFW_KEY_U:
        state->U = action;
        break;
    case GLFW_KEY_V:
        state->V = action;
        break;
    case GLFW_KEY_W:
        state->W = action;
        break;
    case GLFW_KEY_X:
        state->X = action;
        break;
    case GLFW_KEY_Y:
        state->Y = action;
        break;
    case GLFW_KEY_Z:
        state->Z = action;
        break;
    // F keys
    case GLFW_KEY_F1:
        state->F1 = action;
        break;
    case GLFW_KEY_F2:
        state->F2 = action;
        break;
    case GLFW_KEY_F3:
        state->F3 = action;
        break;
    case GLFW_KEY_F4:
        state->F4 = action;
        break;
    case GLFW_KEY_F5:
        state->F5 = action;
        break;
    case GLFW_KEY_F6:
        state->F6 = action;
        break;
    case GLFW_KEY_F7:
        state->F7 = action;
        break;
    case GLFW_KEY_F8:
        state->F8 = action;
        break;
    case GLFW_KEY_F9:
        state->F9 = action;
        break;
    case GLFW_KEY_F10:
        state->F10 = action;
        break;
    case GLFW_KEY_F11:
        state->F11 = action;
        break;
    case GLFW_KEY_F12:
        state->F12 = action;
        break;
    // Special keys
    case GLFW_KEY_LEFT_ALT:
        state->Alt = action;
        break;
    case GLFW_KEY_RIGHT_ALT:
        state->Alt = action;
        break;
    case GLFW_KEY_CAPS_LOCK:
        state->Capslock = action;
        break;
    case GLFW_KEY_LEFT_CONTROL:
        state->Control = action;
        break;
    case GLFW_KEY_RIGHT_CONTROL:
        state->Control = action;
        break;
    case GLFW_KEY_COMMA:
        state->Comma = action;
        break;
    case GLFW_KEY_PERIOD:
        state->Period = action;
        break;
    case GLFW_KEY_TAB:
        state->Tab = action;
        break;
    case GLFW_KEY_ENTER:
        state->Enter = action;
        break;
    case GLFW_KEY_ESCAPE:
        state->Escape = action;
        break;
    default:
        break;
    }
}

}