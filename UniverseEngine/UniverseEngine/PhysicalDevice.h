#pragma once

#include "GraphicsAPI.h"
#include "GraphicsInstance.h"

namespace UniverseEngine {
    class PhysicalDevice {
    public:
        PhysicalDevice(const GraphicsInstance& instance);

#ifdef GRAPHICS_API_VULKAN
    public:
        VkPhysicalDevice GetPhysicalDevice() const;
    private:
        VkPhysicalDevice physicalDevice;
        uint32_t graphicsFamily;
#endif
    };
}  // namespace UniverseEngine