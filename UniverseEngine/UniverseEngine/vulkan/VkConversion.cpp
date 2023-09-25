#include "VkConversion.h"
#ifdef GRAPHICS_API_VULKAN

namespace UniverseEngine {
    VkDescriptorType GetVkDescriptorType(DescriptorType type) {
        switch (type) {
            case DescriptorType::SAMPLER:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            case DescriptorType::COMBINED_IMAGE_SAMPLER:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            case DescriptorType::SAMPLED_IMAGE:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            case DescriptorType::STORAGE_IMAGE:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            case DescriptorType::UNIFORM_BUFFER:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case DescriptorType::STORAGE_BUFFER:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        }
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
    }

    VkShaderStageFlags GetVkShaderStageFlags(GraphicsStageFlags stages) {
        VkShaderStageFlags flags = 0;

        if (stages.test(GetGraphicsStageBitIndex(GraphicsStageFlagBits::VERTEX_STAGE))) {
            flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
        }
        if (stages.test(GetGraphicsStageBitIndex(GraphicsStageFlagBits::FRAGMENT_STAGE))) {
            flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        if (stages.test(GetGraphicsStageBitIndex(GraphicsStageFlagBits::COMPUTE_STAGE))) {
            flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
        }

        return flags;
    }
}  // namespace UniverseEngine
#endif