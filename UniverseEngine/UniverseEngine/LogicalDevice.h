#pragma once

#include "GraphicsAPI.h"
#include "PhysicalDevice.h"

namespace UniverseEngine {
    class LogicalDevice {
    public:
        LogicalDevice(const PhysicalDevice& physicalDevice, bool debug);
        ~LogicalDevice();
        LogicalDevice(const LogicalDevice& other) = delete;
        LogicalDevice& operator=(const LogicalDevice& other) = delete;

#ifdef GRAPHICS_API_VULKAN
    private:
        VkDevice device;
#endif
    };
}  // namespace UniverseEngine