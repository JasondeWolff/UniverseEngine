#include "../Semaphore.h"
#ifdef GRAPHICS_API_GL

#include "../Logging.h"
#include "../LogicalDevice.h"

namespace UniverseEngine {
    Semaphore::Semaphore(std::shared_ptr<LogicalDevice> device) : device(device) {
    }

    Semaphore::Semaphore(Semaphore&& other) noexcept : device(other.device) {
    }

    Semaphore& Semaphore::operator=(Semaphore&& other) noexcept {
        return *this;
    }

    Semaphore::~Semaphore() {
    }
}  // namespace UniverseEngine
#endif