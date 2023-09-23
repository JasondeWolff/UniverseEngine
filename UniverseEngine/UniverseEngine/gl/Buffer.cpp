#include "../Buffer.h"
#ifdef GRAPHICS_API_GL

#include "../Logging.h"
#include "../LogicalDevice.h"

namespace UniverseEngine {
    Buffer::Buffer(const std::string& name, std::shared_ptr<LogicalDevice> device,
                   const PhysicalDevice& physicalDevice, BufferUsage usage, uint64_t size,
                   BufferLocation location)
        : device(device), size(size) {
        
    }

    Buffer::Buffer(Buffer&& other) noexcept
        : device(other.device) {
        
    }

    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        return *this;
    }

    Buffer::~Buffer() {
        
    }

    void* Buffer::Map() {
        return nullptr;
    }

    void Buffer::Unmap() {
        
    }

    uint64_t Buffer::Size() const {
        return this->size;
    }
}  // namespace UniverseEngine
#endif