#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <vector>

#include "../CmdQueue.h"
#include "../Logging.h"
#include "../CmdList.h"

namespace UniverseEngine {
    CmdQueue::CmdQueue(const std::shared_ptr<LogicalDevice> device,
                       const PhysicalDevice& physicalDevice)
        : device(device) {
        vkGetDeviceQueue(device->GetDevice(), physicalDevice.GraphicsFamily(), 0, &this->queue);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags =
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        poolInfo.queueFamilyIndex = physicalDevice.GraphicsFamily();

        UE_ASSERT_MSG(!vkCreateCommandPool(device->GetDevice(), &poolInfo, nullptr, &this->cmdPool),
                      "Failed to create command pool.");
    }

    CmdQueue::~CmdQueue() {
        vkDestroyCommandPool(this->device->GetDevice(), this->cmdPool, nullptr);
    }

    std::shared_ptr<CmdList> CmdQueue::GetCmdList() {
        return std::move(std::shared_ptr<CmdList>(new CmdList(this->device, *this)));
    }

    VkQueue CmdQueue::GetQueue() const {
        return this->queue;
    }

    VkCommandPool CmdQueue::GetCmdPool() const {
        return this->cmdPool;
    }
}  // namespace UniverseEngine
#endif