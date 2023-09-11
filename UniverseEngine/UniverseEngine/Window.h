#pragma once

struct GLFWwindow;

namespace UniverseEngine {
    class Graphics;

    class Window {
    public:
        GLFWwindow* GlfwWindow() const;
        bool ShouldClose() const;
        void Close() const;

    private:
        friend class Graphics;
        Window(const char* title);

        void Update();

        GLFWwindow* glfwWindow;
        unsigned width;
        unsigned height;
    };
}