#include "../DescriptorPool.h"
#ifdef GRAPHICS_API_GL

#include "../DescriptorSet.h"
#include "../Logging.h"
#include "../LogicalDevice.h"

namespace UniverseEngine {
    DescriptorPool::DescriptorPool(const std::shared_ptr<LogicalDevice> device) : device(device) {
    }

    DescriptorPool::~DescriptorPool() {
    }
}  // namespace UniverseEngine
#endif