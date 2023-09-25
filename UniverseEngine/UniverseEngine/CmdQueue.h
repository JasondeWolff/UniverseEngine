#pragma once

#include <memory>
#include <queue>

#include "GraphicsAPI.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"

namespace UniverseEngine {
    class CmdList;
    class Semaphore;
    class Fence;

    enum class QueueType {
        GRAPHICS,
        PRESENT
    };

    class CmdQueue {
    public:
        CmdQueue(const std::shared_ptr<LogicalDevice> device, const PhysicalDevice& physicalDevice,
                 QueueType type);
        ~CmdQueue();
        CmdQueue(const CmdQueue& other) = delete;
        CmdQueue& operator=(const CmdQueue& other) = delete;

        void ProcessCmdLists(bool wait = false);
        std::shared_ptr<CmdList> GetCmdList();
        void SubmitCmdList(std::shared_ptr<CmdList> cmdList, std::shared_ptr<Fence> fence,
                           const std::vector<Semaphore*>& waitSemaphores,
                           const std::vector<Semaphore*>& signalSemaphores);

    private:
        const std::shared_ptr<LogicalDevice> device;

        struct InFlightCmdList {
            std::shared_ptr<Fence> fence;
            std::shared_ptr<CmdList> cmdList;
        };

        std::queue<InFlightCmdList> busyCmdLists;
        std::queue<std::shared_ptr<CmdList>> idleCmdLists;

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