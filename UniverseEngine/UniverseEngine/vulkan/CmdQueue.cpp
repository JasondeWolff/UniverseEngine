#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <vector>

#include "../CmdList.h"
#include "../CmdQueue.h"
#include "../Fence.h"
#include "../Logging.h"
#include "../Semaphore.h"

namespace UniverseEngine {
    CmdQueue::CmdQueue(const std::shared_ptr<LogicalDevice> device,
                       const PhysicalDevice& physicalDevice, QueueType type)
        : device(device) {
        vkGetDeviceQueue(device->GetDevice(), physicalDevice.GraphicsFamily(), 0, &this->queue);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags =
            VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        if (type == QueueType::GRAPHICS)
            poolInfo.queueFamilyIndex = physicalDevice.GraphicsFamily();
        else if (type == QueueType::COMPUTE)
            poolInfo.queueFamilyIndex = physicalDevice.ComputeFamily();
        else
            poolInfo.queueFamilyIndex = physicalDevice.PresentFamily();

        UE_ASSERT_MSG(!vkCreateCommandPool(device->GetDevice(), &poolInfo, nullptr, &this->cmdPool),
                      "Failed to create command pool.");
    }

    CmdQueue::~CmdQueue() {
        this->ProcessCmdLists(true);
        std::queue<std::shared_ptr<CmdList>>().swap(this->idleCmdLists);

        vkDestroyCommandPool(this->device->GetDevice(), this->cmdPool, nullptr);
    }

    void CmdQueue::SubmitCmdList(std::shared_ptr<CmdList> cmdList, std::shared_ptr<Fence> fence,
                                 const std::vector<Semaphore*>& waitSemaphores,
                                 const std::vector<Semaphore*>& signalSemaphores) {
        cmdList->End();

        std::vector<VkSemaphore> vkWaitSemaphores;
        std::vector<VkSemaphore> vkSignalSemaphores;
        for (auto& waitSemaphore : waitSemaphores) {
            vkWaitSemaphores.push_back(waitSemaphore->GetSemaphore());
        }
        for (auto& signalSemaphore : signalSemaphores) {
            vkSignalSemaphores.push_back(signalSemaphore->GetSemaphore());
        }

        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = static_cast<uint32_t>(vkWaitSemaphores.size());
        submitInfo.pWaitSemaphores = vkWaitSemaphores.data();
        submitInfo.signalSemaphoreCount = static_cast<uint32_t>(vkSignalSemaphores.size());
        submitInfo.pSignalSemaphores = vkSignalSemaphores.data();
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmdList->cmdBuffer;
        submitInfo.pWaitDstStageMask = waitStages;

        if (!fence)
            fence = std::make_shared<Fence>(this->device);
        vkQueueSubmit(this->queue, 1, &submitInfo, fence->GetFence());

        this->busyCmdLists.push(InFlightCmdList{fence, cmdList});
    }

    VkQueue CmdQueue::GetQueue() const {
        return this->queue;
    }

    VkCommandPool CmdQueue::GetCmdPool() const {
        return this->cmdPool;
    }
}  // namespace UniverseEngine
#endif