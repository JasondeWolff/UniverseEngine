#include "../DescriptorSetLayout.h"
#ifdef GRAPHICS_API_VULKAN

#include "../LogicalDevice.h"
#include "../Logging.h"
#include "../DescriptorLayoutBinding.h"
#include "VkConversion.h"

namespace UniverseEngine {
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
            vkBindings.push_back(vkBinding);
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