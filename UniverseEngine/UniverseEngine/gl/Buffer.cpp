#include "../Buffer.h"
#ifdef GRAPHICS_API_GL

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "GlDebugNames.h"

namespace UniverseEngine {
    unsigned GetGlBufferIdentifier(BufferUsage usage) {
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::VERTEX_BUFFER))) {
            return GL_ARRAY_BUFFER;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::INDEX_BUFFER))) {
            return GL_ELEMENT_ARRAY_BUFFER;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::UNIFORM_BUFFER))) {
            return GL_UNIFORM_BUFFER;
        }

        UE_FATAL("Unsupported usage");
        return 0;
    }

    Buffer::Buffer(const std::string& name, std::shared_ptr<LogicalDevice> device,
                   const PhysicalDevice& physicalDevice, BufferUsage usage, uint64_t size,
                   BufferLocation location)
        : device(device), size(size), identifier(GetGlBufferIdentifier(usage)) {
        glGenBuffers(1, &this->buffer);
        glBindBuffer(this->identifier, this->buffer);
        GlDebugNames::Set(GL_BUFFER, this->buffer, name);

        if (location == BufferLocation::CPU_TO_GPU)
            this->bufferMemory = malloc(static_cast<size_t>(size));
        else
            this->bufferMemory = nullptr;
    }

    Buffer::Buffer(Buffer&& other) noexcept
        : device(other.device), size(other.size), buffer(other.buffer), bufferMemory(other.bufferMemory), identifier(other.identifier) {
        other.bufferMemory = nullptr;
    }

    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        this->size = other.size;
        this->buffer = other.buffer;
        this->bufferMemory = other.bufferMemory;
        other.bufferMemory = nullptr;
        return *this;
    }

    Buffer::~Buffer() {
        if (this->bufferMemory) {
            free(this->bufferMemory);
            glDeleteBuffers(1, &this->buffer);
        }
    }

    void* Buffer::Map() {
        UE_ASSERT_MSG(this->bufferMemory, "Buffer location is GPU_ONLY.");

        return this->bufferMemory;
    }

    void Buffer::Unmap() {
        UE_ASSERT_MSG(this->bufferMemory, "Buffer location is GPU_ONLY.");

        glBindBuffer(this->identifier, this->buffer);
        glBufferData(this->identifier, this->size,
                     this->bufferMemory, GL_STATIC_DRAW);
    }

    uint64_t Buffer::Size() const {
        return this->size;
    }

    unsigned Buffer::GetBuffer() const {
        return this->buffer;
    }

    unsigned Buffer::GetIdentifier() const {
        return this->identifier;
    }
}  // namespace UniverseEngine
#endif