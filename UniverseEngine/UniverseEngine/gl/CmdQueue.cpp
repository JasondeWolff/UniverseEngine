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
}  // namespace UniverseEngine
#endif