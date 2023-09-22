#pragma once

#include <memory>
#include <vector>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class LogicalDevice;
    struct DescriptorLayoutBinding;

    class DescriptorSetLayout {
    public:
        DescriptorSetLayout(const std::shared_ptr<LogicalDevice> device,
                            const std::vector<DescriptorLayoutBinding>& bindings);
        ~DescriptorSetLayout();
        DescriptorSetLayout(const DescriptorSetLayout& other) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout& other) = delete;

    private:
        const std::shared_ptr<LogicalDevice> device;

#ifdef GRAPHICS_API_VULKAN
    public:
        VkDescriptorSetLayout GetLayout() const;

    private:
        VkDescriptorSetLayout layout;
#endif
    };
}  // namespace UniverseEngine