#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../LogicalDevice.h"
#include "../Logging.h"

namespace UniverseEngine {
    LogicalDevice::LogicalDevice(std::shared_ptr<GraphicsInstance> instance,
                                 const PhysicalDevice& physicalDevice, bool debug)
        : instance(instance) {
        const GLubyte* renderer = glGetString(GL_RENDERER);
        UE_INFO("Using Device: %s", renderer);
    }

    LogicalDevice::~LogicalDevice() {
    }

    void LogicalDevice::WaitIdle() const {
    }
}  // namespace UniverseEngine
#endif