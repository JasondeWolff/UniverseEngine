#pragma once

#include <memory>

#include "GraphicsAPI.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

namespace UniverseEngine {
    class CmdList;

    class CmdQueue {
    public:
        CmdQueue(const std::shared_ptr<LogicalDevice> device, const PhysicalDevice& physicalDevice);
        ~CmdQueue();
        CmdQueue(const CmdQueue& other) = delete;
        CmdQueue& operator=(const CmdQueue& other) = delete;

        std::shared_ptr<CmdList> GetCmdList();

    private:
        const std::shared_ptr<LogicalDevice> device;

#ifdef GRAPHICS_API_VULKAN
    public:
        VkQueue GetQueue() const;
        VkCommandPool GetCmdPool() const;
    private:
        VkQueue queue;
        VkCommandPool cmdPool;
#endif
    };
}  // namespace UniverseEngine