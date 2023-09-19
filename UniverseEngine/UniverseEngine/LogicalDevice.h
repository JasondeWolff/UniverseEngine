#pragma once

#include <memory>

#include "GraphicsAPI.h"
#include "PhysicalDevice.h"

namespace UniverseEngine {
    class LogicalDevice {
    public:
        LogicalDevice(std::shared_ptr<GraphicsInstance> instance,
                      const PhysicalDevice& physicalDevice, bool debug);
        ~LogicalDevice();
        LogicalDevice(const LogicalDevice& other) = delete;
        LogicalDevice& operator=(const LogicalDevice& other) = delete;

    private:
        const std::shared_ptr<GraphicsInstance> instance;

#ifdef GRAPHICS_API_VULKAN
    public:
        VkDevice GetDevice() const;

    private:
        VkDevice device;
#endif
    };
}  // namespace UniverseEngine