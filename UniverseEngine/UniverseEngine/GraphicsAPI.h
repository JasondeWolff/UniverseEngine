#pragma once

#include "Defines.h"

#ifdef GRAPHICS_API_GL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#undef APIENTRY
#elif defined(GRAPHICS_API_VULKAN)
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>
#endif

namespace UniverseEngine {
    class GraphicsAPI {
    public:
        static const char* ShaderDir();
        static const char* ShaderAppendix();
    };
}  // namespace UniverseEngine