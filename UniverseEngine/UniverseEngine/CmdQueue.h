#pragma once

#include "GraphicsAPI.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

namespace UniverseEngine {
    class CmdQueue {
    public:
        CmdQueue(const LogicalDevice& device, const PhysicalDevice& physicalDevice);
        CmdQueue(const CmdQueue& other) = delete;
        CmdQueue& operator=(const CmdQueue& other) = delete;

#ifdef GRAPHICS_API_VULKAN
    public:
        VkQueue GetQueue() const;
    private:
        VkQueue queue;
#endif
    };
}  // namespace UniverseEngine