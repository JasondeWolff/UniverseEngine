#include "../Semaphore.h"
#ifdef GRAPHICS_API_GL

#include "../Logging.h"
#include "../LogicalDevice.h"

namespace UniverseEngine {
    Semaphore::Semaphore(std::shared_ptr<LogicalDevice> device) : device(device) {
    }

    Semaphore::~Semaphore() {
    }
}  // namespace UniverseEngine
#endif