#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Image.h"
#include "../Logging.h"
#include "../Format.h"
#include "../LogicalDevice.h"
#include "VkGraphicsFormat.h"
#include "VkDebugNames.h"

namespace UniverseEngine {
    Image::Image(const std::string& name, std::shared_ptr<LogicalDevice> device,
                 const PhysicalDevice& physicalDevice,
                 uint32_t width, uint32_t height, GraphicsFormat format)
        : device(device), width(width), height(height), imageView(VK_NULL_HANDLE) {
        VkImageCreateInfo createInfo{};
        createInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.imageType = VkImageType::VK_IMAGE_TYPE_2D;
        createInfo.extent.width = width;
        createInfo.extent.height = height;
        createInfo.extent.depth = 1;
        createInfo.mipLevels = 1;
        createInfo.format = VkGraphicsFormat::To(format);
        createInfo.tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
        createInfo.arrayLayers = 1;
        createInfo.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT |
                           VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT;
        createInfo.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
        createInfo.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;

        UE_ASSERT_MSG(!vkCreateImage(device->GetDevice(), &createInfo, nullptr, &this->image),
                      "Failed to create image.");
        VkDebugNames::Set(*device, VK_OBJECT_TYPE_IMAGE,
                          reinterpret_cast<uint64_t>(this->image), name);

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device->GetDevice(), this->image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = physicalDevice.FindMemoryType(
            memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    
        UE_ASSERT_MSG(
            !vkAllocateMemory(device->GetDevice(), &allocInfo, nullptr, &this->imageMemory),
            "Failed to allocate image memory.");
        VkDebugNames::Set(*device, VK_OBJECT_TYPE_DEVICE_MEMORY,
                          reinterpret_cast<uint64_t>(this->imageMemory), Format("%s_MEMORY", name.c_str()));
        vkBindImageMemory(device->GetDevice(), this->image, this->imageMemory, 0);
    
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = this->image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VkGraphicsFormat::To(format);
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        UE_ASSERT_MSG(!vkCreateImageView(device->GetDevice(), &viewInfo, nullptr, &this->imageView),
                      "Failed to create image view");
    }

    Image::Image(std::shared_ptr<LogicalDevice> device, VkImage image, VkImageView imageView,
                 uint32_t width, uint32_t height)
        : device(device),
          image(image),
          imageView(imageView),
          imageMemory(VK_NULL_HANDLE),
          width(width),
          height(height) {
    }

    Image::~Image() {
        vkDestroyImageView(this->device->GetDevice(), this->imageView, nullptr);

        if (this->imageMemory) {
            vkDestroyImage(this->device->GetDevice(), this->image, nullptr);
            vkFreeMemory(this->device->GetDevice(), this->imageMemory, nullptr);
        }
    }

    uint32_t Image::Width() const {
        return this->width;
    }

    uint32_t Image::Height() const {
        return this->height;
    }

    VkImage Image::GetImage() const {
        return this->image;
    }

    VkImageView Image::GetImageView() const {
        return this->imageView;
    }
}  // namespace UniverseEngine
#endif