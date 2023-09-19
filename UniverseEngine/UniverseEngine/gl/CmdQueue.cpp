#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../CmdQueue.h"
#include "../CmdList.h"

namespace UniverseEngine {
    CmdQueue::CmdQueue(const std::shared_ptr<LogicalDevice> device,
                       const PhysicalDevice& physicalDevice) : device(device) {
    }

    CmdQueue::~CmdQueue() {
    }

    std::shared_ptr<CmdList> CmdQueue::GetCmdList() {
        return std::shared_ptr<CmdList>(new CmdList(this->device, *this));
    }
}  // namespace UniverseEngine
#endif