#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <vector>

#include "../CmdQueue.h"

namespace UniverseEngine {
    CmdQueue::CmdQueue(const LogicalDevice& device, const PhysicalDevice& physicalDevice) {
        vkGetDeviceQueue(device.GetDevice(), physicalDevice.GraphicsFamily(), 0, &this->queue);
    }

    VkQueue CmdQueue::GetQueue() const {
        return this->queue;
    }
}  // namespace UniverseEngine
#endif