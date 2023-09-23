#pragma once

#include <memory>
#include <vector>

#include "GraphicsAPI.h"
#include "DescriptorLayoutBinding.h"

namespace UniverseEngine {
    class LogicalDevice;
    class DescriptorPool;
    class DescriptorSetLayout;
    class Buffer;

    class DescriptorSet {
    public:
        DescriptorSet(const std::shared_ptr<LogicalDevice> device,
                      const std::shared_ptr<DescriptorPool> pool,
                      const std::shared_ptr<DescriptorSetLayout> layout);
        ~DescriptorSet();
        DescriptorSet(const DescriptorSet& other) = delete;
        DescriptorSet& operator=(const DescriptorSet& other) = delete;

        void SetBuffer(uint32_t set, uint32_t binding, DescriptorType type, const Buffer& buffer);

    private:
        const std::shared_ptr<LogicalDevice> device;
        const std::shared_ptr<DescriptorPool> pool;
        const std::shared_ptr<DescriptorSetLayout> layout;

#ifdef GRAPHICS_API_VULKAN
    public:
        VkDescriptorSet GetDescriptorSet() const;

    private:
        VkDescriptorSet descriptorSet;
#endif
    };
}  // namespace UniverseEngine