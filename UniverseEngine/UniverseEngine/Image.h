#pragma once

#include <bitset>
#include <memory>
#include <string>

#include "GraphicsAPI.h"
#include "GraphicsFormat.h"

namespace UniverseEngine {
    class LogicalDevice;
    class PhysicalDevice;

    typedef std::bitset<6> ImageUsage;
    enum ImageUsageBits {
        TRANSFER_SRC_IMAGE = 0x0001,
        TRANSFER_DST_IMAGE = 0x0002,
        SAMPLED_IMAGE = 0x0004,
        STORAGE_IMAGE = 0x0008,
        DEPTH_STENCIL_ATTACHMENT = 0x0010,
    };
    size_t GetImageUsageBitIndex(ImageUsageBits bits);

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
              const PhysicalDevice& physicalDevice, uint32_t width, uint32_t height, uint32_t mips,
              ImageUsage usage, GraphicsFormat format, uint32_t arrayLayers = 1);
        ~Image();
        Image(const Image& other) = delete;
        Image& operator=(const Image& other) = delete;

        uint32_t Width() const;
        uint32_t Height() const;
        uint32_t Mips() const;
        uint32_t ArrayLayers() const;
        GraphicsFormat Format() const;

    private:
        uint32_t width;
        uint32_t height;
        uint32_t mips;
        uint32_t arrayLayers;
        GraphicsFormat format;

#ifdef GRAPHICS_API_GL
    public:
        Image(const std::string& name, void* data, uint32_t width, uint32_t height, uint32_t mips,
              GraphicsFormat format);

        unsigned GetTexture() const;
        GLenum Identifier() const;

    private:
        unsigned texture;
        GLenum identifier;
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
        VmaAllocation allocation;
#endif
    };
}  // namespace UniverseEngine