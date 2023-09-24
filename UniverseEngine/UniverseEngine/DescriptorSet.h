#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "DescriptorLayoutBinding.h"
#include "GraphicsAPI.h"

namespace UniverseEngine {
    class LogicalDevice;
    class DescriptorPool;
    class DescriptorSetLayout;
    class Buffer;
    class Image;
    class Sampler;

    class DescriptorSet {
    public:
        DescriptorSet(const std::shared_ptr<LogicalDevice> device,
                      const std::shared_ptr<DescriptorPool> pool,
                      const std::shared_ptr<DescriptorSetLayout> layout);
        ~DescriptorSet();
        DescriptorSet(const DescriptorSet& other) = delete;
        DescriptorSet& operator=(const DescriptorSet& other) = delete;

        void SetBuffer(uint32_t binding, DescriptorType type, const Buffer& buffer);
        void SetImage(uint32_t binding, DescriptorType type, std::shared_ptr<Image> image,
                      std::shared_ptr<Sampler> sampler);

    private:
        const std::shared_ptr<LogicalDevice> device;
        const std::shared_ptr<DescriptorPool> pool;
        const std::shared_ptr<DescriptorSetLayout> layout;

#ifdef GRAPHICS_API_GL
    public:
        const std::unordered_map<uint32_t,
                                 std::pair<std::shared_ptr<Image>, std::shared_ptr<Sampler>>>&
        Images() const;

    private:
        std::unordered_map<uint32_t, std::pair<std::shared_ptr<Image>, std::shared_ptr<Sampler>>>
            images;
#elif defined(GRAPHICS_API_VULKAN)
    public:
        VkDescriptorSet GetDescriptorSet() const;

    private:
        VkDescriptorSet descriptorSet;
#endif
    };
}  // namespace UniverseEngine