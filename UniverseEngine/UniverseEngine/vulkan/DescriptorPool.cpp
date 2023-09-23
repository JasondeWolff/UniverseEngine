#include "../DescriptorPool.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../DescriptorSet.h"

namespace UniverseEngine {
    DescriptorPool::DescriptorPool(const std::shared_ptr<LogicalDevice> device) : device(device) {
        std::vector<VkDescriptorPoolSize> poolSizes = {
            VkDescriptorPoolSize{VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 64}};

        VkDescriptorPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        createInfo.flags =
            VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        createInfo.maxSets = 512;
        createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        createInfo.pPoolSizes = poolSizes.data();

        UE_ASSERT_MSG(
            !vkCreateDescriptorPool(device->GetDevice(), &createInfo, nullptr, &this->pool),
            "Failed to create descriptor pool.");
    }

    DescriptorPool::~DescriptorPool() {
        vkDestroyDescriptorPool(this->device->GetDevice(), this->pool, nullptr);
    }

    VkDescriptorPool DescriptorPool::GetPool() const {
        return this->pool;
    }
}  // namespace UniverseEngine
#endif