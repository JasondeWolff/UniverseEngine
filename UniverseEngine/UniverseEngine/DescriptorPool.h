#pragma once

#include <memory>
#include <vector>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class LogicalDevice;
    class DescriptorSet;
    class DescriptorSetLayout;

    class DescriptorPool {
    public:
        DescriptorPool(const std::shared_ptr<LogicalDevice> device);
        ~DescriptorPool();
        DescriptorPool(const DescriptorPool& other) = delete;
        DescriptorPool& operator=(const DescriptorPool& other) = delete;

    private:
        const std::shared_ptr<LogicalDevice> device;

#ifdef GRAPHICS_API_VULKAN
    public:
        VkDescriptorPool GetPool() const;

    private:
        VkDescriptorPool pool;
#endif
    };
}  // namespace UniverseEngine