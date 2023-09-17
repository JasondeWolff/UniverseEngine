#pragma once

#include "GraphicsAPI.h"
#include "Window.h"

namespace UniverseEngine {
    class GraphicsAPI;

    class GraphicsInstance {
    public:
        ~GraphicsInstance();
        GraphicsInstance(const GraphicsInstance& other) = delete;
        GraphicsInstance& operator=(const GraphicsInstance& other) = delete;

    private:
        friend class GraphicsAPI;
        GraphicsInstance(const Window& window, bool debug);

#ifdef GRAPHICS_API_GL
#elif defined(GRAPHICS_API_VULKAN)
    public:
        VkInstance GetInstance() const;
        VkSurfaceKHR GetSurface() const;
    private:
        VkInstance instance;
        VkSurfaceKHR surface;
#endif
    };
}  // namespace UniverseEngine