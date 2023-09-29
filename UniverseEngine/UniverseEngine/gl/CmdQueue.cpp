#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../CmdList.h"
#include "../CmdQueue.h"

namespace UniverseEngine {
    CmdQueue::CmdQueue(const std::shared_ptr<LogicalDevice> device,
                       const PhysicalDevice& physicalDevice, QueueType type)
        : device(device) {
    }

    CmdQueue::~CmdQueue() {
    }

    void CmdQueue::SubmitCmdList(std::shared_ptr<CmdList> cmdList, std::shared_ptr<Fence> fence,
                                 const std::vector<Semaphore*>& waitSemaphores,
                                 const std::vector<Semaphore*>& signalSemaphores) {
        this->busyCmdLists.push(InFlightCmdList{fence, cmdList});
    }
}  // namespace UniverseEngine
#endif