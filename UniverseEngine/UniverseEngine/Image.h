#pragma once

#include <memory>

#include "GraphicsAPI.h"
#include "GraphicsFormat.h"

namespace UniverseEngine {
    class LogicalDevice;

    class Image {
    public:
        Image(std::shared_ptr<LogicalDevice> device, uint32_t width, uint32_t height,
              GraphicsFormat format);
        ~Image();
        Image(const Image& other) = delete;
        Image& operator=(const Image& other) = delete;

        uint32_t Width() const;
        uint32_t Height() const;

    private:
        uint32_t width;
        uint32_t height;
#ifdef GRAPHICS_API_GL

#elif defined(GRAPHICS_API_VULKAN)
    public:
        Image(std::shared_ptr<LogicalDevice> device, VkImage image, VkImageView imageView, uint32_t width, uint32_t height);

        VkImage GetImage() const;
        VkImageView GetImageView() const;

    private:
        const std::shared_ptr<LogicalDevice> device;

        VkImage image;
        VkImageView imageView;
#endif
    };
}  // namespace UniverseEngine