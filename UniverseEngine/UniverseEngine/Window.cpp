#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Logging.h"

namespace UniverseEngine {
    void GlfwErrorCallback(int error, const char* description) {
        UE_FATAL(description);
    }

    Window::Window(const char* title)
        : width(640), height(480) {
        UE_ASSERT_MSG(glfwInit(), "Failed to init glfw.");
        glfwSetErrorCallback(GlfwErrorCallback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        this->glfwWindow =
            glfwCreateWindow(static_cast<int>(this->width), static_cast<int>(this->height), title, NULL, NULL);
        UE_ASSERT_MSG(this->glfwWindow, "Failed to create window.");

        glfwMakeContextCurrent(this->glfwWindow);
        glewInit();
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

    void Window::Update() {
        glfwSwapBuffers(this->glfwWindow);
        glfwPollEvents();
    }
}  // namespace UniverseEngine