#include "../Semaphore.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "VkDebugNames.h"

namespace UniverseEngine {
    Semaphore::Semaphore(const std::string& name, std::shared_ptr<LogicalDevice> device) : device(device) {
        VkSemaphoreCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        UE_ASSERT_MSG(
            !vkCreateSemaphore(device->GetDevice(), &createInfo, nullptr, &this->semaphore),
            "Failed to create semaphore.");

        VkDebugNames::Set(*device, VkObjectType::VK_OBJECT_TYPE_SEMAPHORE,
                          reinterpret_cast<uint64_t>(this->semaphore), name);
    }

    Semaphore::Semaphore(Semaphore&& other) noexcept : device(other.device), semaphore(other.semaphore) {
        other.semaphore = VK_NULL_HANDLE;
    }

    Semaphore& Semaphore::operator=(Semaphore&& other) noexcept {
        this->semaphore = other.semaphore;
        other.semaphore = VK_NULL_HANDLE;
        return *this;
    }

    Semaphore::~Semaphore() {
        if (this->semaphore) {
            vkDestroySemaphore(this->device->GetDevice(), this->semaphore, nullptr);
        }
    }

    VkSemaphore Semaphore::GetSemaphore() const {
        return this->semaphore;
    }
}  // namespace UniverseEngine
#endif