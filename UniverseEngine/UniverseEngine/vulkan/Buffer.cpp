#include "../Buffer.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Logging.h"
#include "../LogicalDevice.h"
#include "VkDebugNames.h"

namespace UniverseEngine {
    VkBufferUsageFlags GetVkUsageFlags(BufferUsage usage) {
        VkBufferUsageFlags flags = 0;

        if (usage.test(GetBufferUsageBitIndex(BufferUsageBit::TRANSFER_SRC))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBit::TRANSFER_DST))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBit::UNIFORM_BUFFER))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBit::STORAGE_BUFFER))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBit::INDEX_BUFFER))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        }
        if (usage.test(GetBufferUsageBitIndex(BufferUsageBit::VERTEX_BUFFER))) {
            flags |= VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        }

        return flags;
    }

    uint32_t FindMemoryType(const PhysicalDevice& physicalDevice, uint32_t typeFilter,
                            VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice.GetPhysicalDevice(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        UE_FATAL("Failed to find suitable memory type.");
        return 0;
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

        UE_ASSERT_MSG(!vkCreateBuffer(device->GetDevice(), &createInfo, nullptr, &this->buffer),
                      "Failed to create buffer.");
        VkDebugNames::Set(*device, VK_OBJECT_TYPE_BUFFER, reinterpret_cast<uint64_t>(this->buffer),
                          name);

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device->GetDevice(), this->buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(
            physicalDevice, memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        UE_ASSERT_MSG(
            !vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &this->bufferMemory),
            "Failed to allocate buffer memory.");
        VkDebugNames::Set(*device, VK_OBJECT_TYPE_DEVICE_MEMORY,
                          reinterpret_cast<uint64_t>(this->bufferMemory), name);
        vkBindBufferMemory(device->GetDevice(), this->buffer, this->bufferMemory, 0);
    }

    Buffer::Buffer(Buffer&& other) noexcept : device(other.device), buffer(other.buffer), bufferMemory(other.bufferMemory) {
        other.buffer = VK_NULL_HANDLE;
        other.bufferMemory = VK_NULL_HANDLE;
    }

    Buffer& Buffer::operator=(Buffer&& other) noexcept {
        this->buffer = other.buffer;
        this->bufferMemory = other.bufferMemory;
        other.buffer = VK_NULL_HANDLE;
        other.bufferMemory = VK_NULL_HANDLE;
        return *this;
    }

    Buffer::~Buffer() {
        if (this->buffer) {
            vkDestroyBuffer(this->device->GetDevice(), this->buffer, nullptr);
            vkFreeMemory(this->device->GetDevice(), this->bufferMemory, nullptr);
        }
    }

    void* Buffer::Map() {
        void* data;
        vkMapMemory(this->device->GetDevice(), this->bufferMemory, 0, this->size, 0, &data);
        return data;
    }

    void Buffer::Unmap() {
        vkUnmapMemory(this->device->GetDevice(), this->bufferMemory);
    }

    uint64_t Buffer::Size() const {
        return this->size;
    }

    VkBuffer Buffer::GetBuffer() const {
        return this->buffer;
    }
}  // namespace UniverseEngine
#endif