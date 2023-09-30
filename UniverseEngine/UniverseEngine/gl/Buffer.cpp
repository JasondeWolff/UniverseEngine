#include "../Buffer.h"
#ifdef GRAPHICS_API_GL

#include "../Format.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "../Mesh.h"
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
        : device(device), size(size), identifier(GetGlBufferIdentifier(usage)), vao(0) {
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::VERTEX_BUFFER))) {
            glGenVertexArrays(1, &this->vao);
            glBindVertexArray(this->vao);
            GlDebugNames::Set(GL_VERTEX_ARRAY, this->vao, Format("%s_VAO", name.c_str()));
        }

        glGenBuffers(1, &this->buffer);
        glBindBuffer(this->identifier, this->buffer);
        GlDebugNames::Set(GL_BUFFER, this->buffer, name);

        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::VERTEX_BUFFER))) {
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  (void*)offsetof(Vertex, texCoord));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  (void*)offsetof(Vertex, tangent));
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  (void*)offsetof(Vertex, color));
        }

        if (location == BufferLocation::CPU_TO_GPU)
            this->bufferMemory = malloc(static_cast<size_t>(size));
        else
            this->bufferMemory = nullptr;
    }

    Buffer::Buffer(Buffer&& other) noexcept
        : device(other.device),
          size(other.size),
          buffer(other.buffer),
          bufferMemory(other.bufferMemory),
          identifier(other.identifier) {
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

            if (this->vao) {
                glDeleteVertexArrays(1, &this->vao);
            }
        }
    }

    void* Buffer::Map() {
        UE_ASSERT_MSG(this->bufferMemory, "Buffer location is GPU_ONLY.");

        return this->bufferMemory;
    }

    void Buffer::Unmap() {
        UE_ASSERT_MSG(this->bufferMemory, "Buffer location is GPU_ONLY.");

        glBindBuffer(this->identifier, this->buffer);
        glBufferData(this->identifier, this->size, this->bufferMemory, GL_STATIC_DRAW);
    }

    uint64_t Buffer::Size() const {
        return this->size;
    }

    unsigned Buffer::GetBuffer() const {
        return this->buffer;
    }

    unsigned Buffer::GetVao() const {
        return this->vao;
    }

    unsigned Buffer::GetIdentifier() const {
        return this->identifier;
    }
}  // namespace UniverseEngine
#endif