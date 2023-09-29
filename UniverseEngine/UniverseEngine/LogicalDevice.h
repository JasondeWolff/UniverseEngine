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

        void WaitIdle() const;

    private:
        const std::shared_ptr<GraphicsInstance> instance;

#ifdef GRAPHICS_API_VULKAN
    public:
        VkDevice GetDevice() const;
        VmaAllocator GetAllocator() const;

    private:
        VkDevice device;
        VmaAllocator allocator;
#endif
    };
}  // namespace UniverseEngine