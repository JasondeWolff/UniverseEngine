#pragma once

// #define GRAPHICS_API_GL
#define GRAPHICS_API_VULKAN

#ifdef GRAPHICS_API_GL
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#elif defined(GRAPHICS_API_VULKAN)
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#endif

#include <memory>

#include "GraphicsInstance.h"

namespace UniverseEngine {
    class GraphicsAPI {
    public:
        static std::unique_ptr<GraphicsInstance> Init(const Window& window, bool debug);
        static const char* ShaderDir();
    };
}  // namespace UniverseEngine