#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../LogicalDevice.h"

namespace UniverseEngine {
    LogicalDevice::LogicalDevice(std::shared_ptr<GraphicsInstance> instance,
                                 const PhysicalDevice& physicalDevice, bool debug)
        : instance(instance) {
    }

    LogicalDevice::~LogicalDevice() {
    }

    void LogicalDevice::WaitIdle() const {
    }
}  // namespace UniverseEngine
#endif