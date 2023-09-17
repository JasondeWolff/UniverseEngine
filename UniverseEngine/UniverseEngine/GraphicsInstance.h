#pragma once

#include "GraphicsAPI.h"

#ifdef GRAPHICS_API_VULKAN
#include <vulkan/vulkan.h>
#endif

namespace UniverseEngine {
    class GraphicsAPI;

    class GraphicsInstance {
    public:
        ~GraphicsInstance();
        GraphicsInstance(const GraphicsInstance& other) = delete;
        GraphicsInstance& operator=(const GraphicsInstance& other) = delete;

    private:
        friend class GraphicsAPI;
        GraphicsInstance(bool debug);

#ifdef GRAPHICS_API_GL
#elif defined(GRAPHICS_API_VULKAN)
        VkInstance instance;
#endif
    };
}  // namespace UniverseEngine