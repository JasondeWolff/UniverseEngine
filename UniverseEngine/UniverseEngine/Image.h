#pragma once

#include <memory>
#include <string>

#include "GraphicsAPI.h"
#include "GraphicsFormat.h"

namespace UniverseEngine {
    class LogicalDevice;
    class PhysicalDevice;

    enum class ImageLayout {
        UNDEFINED,
        GENERAL,
        COLOR_ATTACHMENT_OPTIMAL,
        DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        DEPTH_STENCIL_READ_ONLY_OPTIMAL,
        SHADER_READ_ONLY_OPTIMAL,
        TRANSFER_SRC_OPTIMAL,
        TRANSFER_DST_OPTIMAL,
    };

    class Image {
    public:
        Image(const std::string& name, std::shared_ptr<LogicalDevice> device,
              const PhysicalDevice& physicalDevice, uint32_t width, uint32_t height,
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
    public:
        Image(void* data, uint32_t width, uint32_t height, GraphicsFormat format);

        unsigned GetTexture() const;

    private:
        unsigned texture;
#elif defined(GRAPHICS_API_VULKAN)
    public:
        Image(std::shared_ptr<LogicalDevice> device, VkImage image, VkImageView imageView,
              uint32_t width, uint32_t height);

        VkImage GetImage() const;
        VkImageView GetImageView() const;

    private:
        const std::shared_ptr<LogicalDevice> device;

        VkImage image;
        VkImageView imageView;
        VkDeviceMemory imageMemory;
#endif
    };
}  // namespace UniverseEngine