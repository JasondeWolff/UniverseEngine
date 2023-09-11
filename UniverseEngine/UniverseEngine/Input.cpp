#include "Input.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine.h"

namespace UniverseEngine {
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    Input::Input() {
        const Window& window = Engine::GetGraphics().GetWindow();
        GLFWwindow* glfwWindow = window.GlfwWindow();

        glfwSetKeyCallback(glfwWindow, KeyCallback);
    }
}