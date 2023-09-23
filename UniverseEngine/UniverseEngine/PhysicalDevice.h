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
        uint32_t GraphicsFamily() const;
        uint32_t PresentFamily() const;

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    private:
        VkPhysicalDevice physicalDevice;
        uint32_t graphicsFamily;
        uint32_t presentFamily;
#endif
    };
}  // namespace UniverseEngine