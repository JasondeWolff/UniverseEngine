#include "../Fence.h"
#ifdef GRAPHICS_API_GL

#include "../Logging.h"
#include "../LogicalDevice.h"

namespace UniverseEngine {
    Fence::Fence(std::shared_ptr<LogicalDevice> device, bool signaled) : device(device) {
    }

    Fence::Fence(Fence&& other) noexcept : device(other.device) {
    }

    Fence& Fence::operator=(Fence&& other) noexcept {
        return *this;
    }

    Fence::~Fence() {
    }

    bool Fence::IsComplete() const {
        return true;
    }

    void Fence::Wait() const {
    }

    void Fence::Reset() {
    }
}  // namespace UniverseEngine
#endif