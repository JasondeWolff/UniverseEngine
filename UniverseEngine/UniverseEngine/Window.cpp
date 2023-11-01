#include "Window.h"

#include "Defines.h"

#ifdef GRAPHICS_API_GL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#elif defined(GRAPHICS_API_VULKAN)
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

#include <imgui.h>

#include "Engine.h"
#include "GraphicsAPI.h"
#include "Logging.h"

namespace UniverseEngine {
    void GlfwErrorCallback(int error, const char* description) {
        UE_FATAL(description);
    }

    void Window::GlfwWindowSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->width = static_cast<unsigned>(width);
        window->height = static_cast<unsigned>(height);
        window->wasResized = true;

        ImGui::GetIO().DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
    }

    void Window::GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key != GLFW_KEY_UNKNOWN) {
            Engine::GetInput().keys[static_cast<size_t>(key)] = (action != GLFW_RELEASE);
        }
    }

    void Window::GlfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
        Engine::GetInput().mousePosition =
            glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));

        static bool firstCall = true;
        if (firstCall) {
            firstCall = false;

            Engine::GetInput().oldMousePosition = Engine::GetInput().mousePosition;
        }

        ImGui::GetIO().AddMousePosEvent(static_cast<float>(xpos), static_cast<float>(ypos));
    }

    void Window::GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        Engine::GetInput().mouseButtons[static_cast<size_t>(button)] = (action != GLFW_RELEASE);
    
        ImGui::GetIO().AddMouseButtonEvent(button, action != GLFW_RELEASE);
    }

    Window::Window(const char* title) : width(1600), height(900), wasResized(false) {
        UE_ASSERT_MSG(glfwInit(), "Failed to init glfw.");
        glfwSetErrorCallback(GlfwErrorCallback);

#ifdef GRAPHICS_API_GL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#elif defined(GRAPHICS_API_VULKAN)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif
        this->glfwWindow = glfwCreateWindow(static_cast<int>(this->width),
                                            static_cast<int>(this->height), title, NULL, NULL);
        UE_ASSERT_MSG(this->glfwWindow, "Failed to create window.");

#ifdef GRAPHICS_API_GL
        glfwMakeContextCurrent(this->glfwWindow);
        glfwSwapInterval(0);
#endif

        glfwSetWindowUserPointer(this->glfwWindow, static_cast<void*>(this));
        glfwSetWindowSizeCallback(this->glfwWindow, Window::GlfwWindowSizeCallback);
        glfwSetKeyCallback(this->glfwWindow, Window::GlfwKeyCallback);
        glfwSetCursorPosCallback(this->glfwWindow, Window::GlfwCursorPositionCallback);
        glfwSetMouseButtonCallback(this->glfwWindow, Window::GlfwMouseButtonCallback);

        ImGui::SetCurrentContext(ImGui::CreateContext());
        ImGui::GetIO().DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
    }

    GLFWwindow* Window::GlfwWindow() const {
        return this->glfwWindow;
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(this->glfwWindow);
    }

    void Window::Close() const {
        glfwSetWindowShouldClose(this->glfwWindow, GLFW_TRUE);
    }

    bool Window::IsMinimized() const {
        return this->width == 0 || this->height == 0;
    }

    uint32_t Window::Width() const {
        return this->width;
    }

    uint32_t Window::Height() const {
        return this->height;
    }

    bool Window::WasResized() const {
        return this->wasResized;
    }

    void Window::Update() {
        this->wasResized = false;

#ifdef GRAPHICS_API_GL
        glfwSwapBuffers(this->glfwWindow);
#endif

        glfwPollEvents();
    }
}  // namespace UniverseEngine