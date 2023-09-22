#include "../DescriptorSetLayout.h"
#ifdef GRAPHICS_API_VULKAN

#include "../LogicalDevice.h"
#include "../Logging.h"
#include "../DescriptorLayoutBinding.h"

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

    VkShaderStageFlags GetVkShaderStageFlags(DescriptorStageFlags stages) {
        VkShaderStageFlags flags = 0;

        if (stages.test(GetDescriptorStageBitIndex(DescriptorStageFlagBits::VERTEX))) {
            flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
        }
        if (stages.test(GetDescriptorStageBitIndex(DescriptorStageFlagBits::FRAGMENT))) {
            flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        if (stages.test(GetDescriptorStageBitIndex(DescriptorStageFlagBits::COMPUTE))) {
            flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
        }

        return flags;
    }

    DescriptorSetLayout::DescriptorSetLayout(const std::shared_ptr<LogicalDevice> device,
                                             const std::vector<DescriptorLayoutBinding>& bindings)
        : device(device) {

        std::vector<VkDescriptorSetLayoutBinding> vkBindings;
        for (auto binding : bindings) {
            VkDescriptorSetLayoutBinding vkBinding{};
            vkBinding.binding = binding.binding;
            vkBinding.descriptorCount = 1;
            vkBinding.descriptorType = GetVkDescriptorType(binding.type);
            vkBinding.stageFlags = GetVkShaderStageFlags(binding.stageFlags);
        }

        VkDescriptorSetLayoutCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.bindingCount = static_cast<uint32_t>(vkBindings.size());
        createInfo.pBindings = vkBindings.data();

        UE_ASSERT_MSG(
            !vkCreateDescriptorSetLayout(device->GetDevice(), &createInfo, nullptr, &this->layout),
            "Failed to create descriptor set layout.");
    }

    DescriptorSetLayout::~DescriptorSetLayout(){
        vkDestroyDescriptorSetLayout(this->device->GetDevice(), this->layout, nullptr);
    }

    VkDescriptorSetLayout DescriptorSetLayout::GetLayout() const {
        return this->layout;
    }
}
#endif