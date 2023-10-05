#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Format.h"
#include "../Image.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "VkDebugNames.h"
#include "VkGraphicsFormat.h"

namespace UniverseEngine {
    VkImageUsageFlags GetVkImageUsageFlags(ImageUsage usage) {
        VkImageUsageFlags flags = 0;

        if (usage.test(GetImageUsageBitIndex(ImageUsageBits::TRANSFER_SRC_IMAGE))) {
            flags |= VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }
        if (usage.test(GetImageUsageBitIndex(ImageUsageBits::TRANSFER_DST_IMAGE))) {
            flags |= VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }
        if (usage.test(GetImageUsageBitIndex(ImageUsageBits::SAMPLED_IMAGE))) {
            flags |= VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT;
        }
        if (usage.test(GetImageUsageBitIndex(ImageUsageBits::STORAGE_IMAGE))) {
            flags |= VkImageUsageFlagBits::VK_IMAGE_USAGE_STORAGE_BIT;
        }
        if (usage.test(GetImageUsageBitIndex(ImageUsageBits::COLOR_ATTACHMENT))) {
            flags |= VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }
        if (usage.test(GetImageUsageBitIndex(ImageUsageBits::DEPTH_STENCIL_ATTACHMENT))) {
            flags |= VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        }

        return flags;
    }

    Image::Image(const std::string& name, std::shared_ptr<LogicalDevice> device,
                 const PhysicalDevice& physicalDevice, uint32_t width, uint32_t height,
                 uint32_t mips, ImageUsage usage, GraphicsFormat format, uint32_t arrayLayers)
        : device(device),
          width(width),
          height(height),
          mips(mips),
          arrayLayers(arrayLayers),
          format(format),
          imageView(VK_NULL_HANDLE) {
        VkImageCreateInfo createInfo{};
        createInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.imageType = VK_IMAGE_TYPE_2D;
        createInfo.extent.width = width;
        createInfo.extent.height = height;
        createInfo.extent.depth = 1;
        createInfo.mipLevels = mips;
        createInfo.format = VkGraphicsFormat::To(format);
        createInfo.tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
        createInfo.arrayLayers = arrayLayers;
        createInfo.usage = GetVkImageUsageFlags(usage);
        createInfo.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
        createInfo.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
        if (arrayLayers > 1)
            createInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

        VmaAllocationCreateInfo allocCreateInfo{};
        allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;

        UE_ASSERT_MSG(!vmaCreateImage(device->GetAllocator(), &createInfo, &allocCreateInfo,
                                      &this->image, &this->allocation, nullptr),
                      "Failed to create image.");
        VkDebugNames::Set(*device, VK_OBJECT_TYPE_IMAGE, reinterpret_cast<uint64_t>(this->image),
                          name);

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = this->image;
        viewInfo.viewType = (arrayLayers == 1) ? VK_IMAGE_VIEW_TYPE_2D : VK_IMAGE_VIEW_TYPE_CUBE;
        viewInfo.format = VkGraphicsFormat::To(format);
        viewInfo.subresourceRange.aspectMask =
            IsDepthFormat(format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mips;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = arrayLayers;

        UE_ASSERT_MSG(!vkCreateImageView(device->GetDevice(), &viewInfo, nullptr, &this->imageView),
                      "Failed to create image view");
    }

    Image::Image(std::shared_ptr<LogicalDevice> device, VkImage image, VkImageView imageView,
                 uint32_t width, uint32_t height)
        : device(device),
          image(image),
          imageView(imageView),
          allocation(VK_NULL_HANDLE),
          width(width),
          height(height),
          mips(1),
          arrayLayers(1) {
    }

    Image::~Image() {
        vkDestroyImageView(this->device->GetDevice(), this->imageView, nullptr);

        if (this->allocation) {
            vmaDestroyImage(this->device->GetAllocator(), this->image, this->allocation);
        }
    }

    VkImage Image::GetImage() const {
        return this->image;
    }

    VkImageView Image::GetImageView() const {
        return this->imageView;
    }
}  // namespace UniverseEngine
#endif