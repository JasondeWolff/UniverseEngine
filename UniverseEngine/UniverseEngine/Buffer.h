#pragma once

#include <bitset>
#include <memory>
#include <string>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class LogicalDevice;
    class PhysicalDevice;

    typedef std::bitset<6> BufferUsage;
    enum BufferUsageBit {
        TRANSFER_SRC = 0x0001,
        TRANSFER_DST = 0x0002,
        UNIFORM_BUFFER = 0x0004,
        STORAGE_BUFFER = 0x0008,
        INDEX_BUFFER = 0x0010,
        VERTEX_BUFFER = 0x0020
    };
    size_t GetBufferUsageBitIndex(BufferUsageBit bit);

    enum class BufferLocation { GPU_ONLY, CPU_TO_GPU };

    class Buffer {
    public:
        Buffer(const std::string& name, std::shared_ptr<LogicalDevice> device,
               const PhysicalDevice& physicalDevice, BufferUsage usage, uint64_t size,
               BufferLocation location);
        ~Buffer();
        explicit Buffer(const Buffer& other) = delete;
        Buffer& operator=(const Buffer& other) = delete;
        explicit Buffer(Buffer&& other) noexcept;
        Buffer& operator=(Buffer&& other) noexcept;

        void* Map();
        void Unmap();

        uint64_t Size() const;

    private:
        const std::shared_ptr<LogicalDevice> device;

        uint64_t size;

#ifdef GRAPHICS_API_VULKAN
    public:
        VkBuffer GetBuffer() const;

    private:
        VkBuffer buffer;
        VkDeviceMemory bufferMemory;
#endif
    };
}  // namespace UniverseEngine