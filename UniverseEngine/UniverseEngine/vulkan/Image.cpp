#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../Image.h"
#include "../Logging.h"
#include "../LogicalDevice.h"
#include "VkGraphicsFormat.h"

namespace UniverseEngine {
    Image::Image(std::shared_ptr<LogicalDevice> device, uint32_t width, uint32_t height,
                 GraphicsFormat format)
        : device(device) {
        VkImageCreateInfo createInfo{};
        createInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.imageType = VkImageType::VK_IMAGE_TYPE_2D;
        createInfo.extent.width = width;
        createInfo.extent.height = height;
        createInfo.extent.depth = 1;
        createInfo.mipLevels = 1;
        createInfo.format = VkGraphicsFormat::To(format);
        createInfo.tiling = VkImageTiling::VK_IMAGE_TILING_LINEAR;
        createInfo.arrayLayers = 1;
        createInfo.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_STORAGE_BIT;
        UE_FATAL("");
    }

    Image::Image(std::shared_ptr<LogicalDevice> device, VkImage image, VkImageView imageView,
                 uint32_t width, uint32_t height)
        : device(device), image(image), imageView(imageView), width(width), height(height) {
    }

    Image::~Image() {
        vkDestroyImageView(this->device->GetDevice(), this->imageView, nullptr);
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