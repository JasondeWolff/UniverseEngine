#include "../Semaphore.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "../LogicalDevice.h"

namespace UniverseEngine {
    Semaphore::Semaphore(std::shared_ptr<LogicalDevice> device) : device(device) {
        VkSemaphoreCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        UE_ASSERT_MSG(
            !vkCreateSemaphore(device->GetDevice(), &createInfo, nullptr, &this->semaphore),
            "Failed to create semaphore.");
    }

    Semaphore::~Semaphore() {
        vkDestroySemaphore(this->device->GetDevice(), this->semaphore, nullptr);
    }

    VkSemaphore Semaphore::GetSemaphore() const {
        return this->semaphore;
    }
}  // namespace UniverseEngine
#endif