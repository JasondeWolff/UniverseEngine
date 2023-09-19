#include "../Fence.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "../LogicalDevice.h"

namespace UniverseEngine {
    Fence::Fence(std::shared_ptr<LogicalDevice> device, bool signaled) : device(device) {
        VkFenceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.flags = signaled ? VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT : 0;

        UE_ASSERT_MSG(!vkCreateFence(device->GetDevice(), &createInfo, nullptr, &this->fence),
                      "Failed to create fence.");
    }

    Fence::~Fence() {
        vkDestroyFence(this->device->GetDevice(), this->fence, nullptr);
    }

    bool Fence::IsComplete() const {
        return vkGetFenceStatus(this->device->GetDevice(), this->fence) == VK_SUCCESS;
    }

    void Fence::Wait() const {
        vkWaitForFences(this->device->GetDevice(), 1, &this->fence, true,
                        std::numeric_limits<uint64_t>::max());
    }

    void Fence::Reset() {
        vkResetFences(this->device->GetDevice(), 1, &this->fence);
    }

    VkFence Fence::GetFence() const {
        return this->fence;
    }
}  // namespace UniverseEngine
#endif