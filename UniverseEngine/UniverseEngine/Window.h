#pragma once

#include <cstdint>

struct GLFWwindow;

namespace UniverseEngine {
    class Graphics;

    class Window {
    public:
        GLFWwindow* GlfwWindow() const;
        bool ShouldClose() const;
        void Close() const;

        uint32_t Width() const;
        uint32_t Height() const;

    private:
        friend class Graphics;
        Window(const char* title);

        void Update();

        GLFWwindow* glfwWindow;
        unsigned width;
        unsigned height;

        static void GlfwWindowSizeCallback(GLFWwindow* glfwWindow, int width, int height);
        static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void GlfwCursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
        static void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    };
}