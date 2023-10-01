#include "../Buffer.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "VkDebugNames.h"

namespace UniverseEngine {
    VkBufferUsageFlags GetVkUsageFlags(BufferUsage usage) {
        VkBufferUsageFlags flags = 0;

        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::TRANSFER_SRC_BUFFER))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::TRANSFER_DST_BUFFER))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::UNIFORM_BUFFER))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::STORAGE_BUFFER))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::INDEX_BUFFER))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBits::VERTEX_BUFFER))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        }

        return flags;
    }

    Buffer::Buffer(const std::string& name, std::shared_ptr<LogicalDevice> device,
                   const PhysicalDevice& physicalDevice, BufferUsage usage, uint64_t size,
                   BufferLocation location)
        : device(device), size(size) {
        VkBufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.size = size;
        createInfo.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
        createInfo.usage = GetVkUsageFlags(usage);

        VmaAllocationCreateInfo allocCreateInfo{};
        if (location == BufferLocation::CPU_TO_GPU)
            allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
        allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;

        UE_ASSERT_MSG(!vmaCreateBuffer(device->GetAllocator(), &createInfo, &allocCreateInfo,
                                       &this->buffer, &this->allocation, nullptr),
                      "Failed to create buffer.");
        VkDebugNames::Set(*device, VK_OBJECT_TYPE_BUFFER, reinterpret_cast<uint64_t>(this->buffer),
                          name);
    }

    Buffer::Buffer(Buffer&& other) noexcept
        : device(other.device), size(other.size), buffer(other.buffer), allocation(other.allocation) {
        other.buffer = VK_NULL_HANDLE;
        other.allocation = VK_NULL_HANDLE;
    }

    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        this->size = other.size;
        this->buffer = other.buffer;
        this->allocation = other.allocation;
        other.buffer = VK_NULL_HANDLE;
        other.allocation = VK_NULL_HANDLE;
        return *this;
    }

    Buffer::~Buffer() {
        if (this->buffer) {
            vmaDestroyBuffer(this->device->GetAllocator(), this->buffer, this->allocation);
        }
    }

    void* Buffer::Map() {
        void* data;
        vmaMapMemory(this->device->GetAllocator(), this->allocation, &data);
        return data;
    }

    void Buffer::Unmap() {
        vmaUnmapMemory(this->device->GetAllocator(), this->allocation);
        vmaFlushAllocation(this->device->GetAllocator(), this->allocation, 0, this->size);
    }

    uint64_t Buffer::Size() const {
        return this->size;
    }

    VkBuffer Buffer::GetBuffer() const {
        return this->buffer;
    }
}  // namespace UniverseEngine
#endif