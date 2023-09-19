#pragma once

#include <memory>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class RenderPass;
    class LogicalDevice;
    class Image;

    class Framebuffer {
    public:
        Framebuffer(std::shared_ptr<LogicalDevice> device, std::shared_ptr<Image> image,
                    std::shared_ptr<RenderPass> renderPass);
        ~Framebuffer();

        explicit Framebuffer(const Framebuffer& other) = delete;
        Framebuffer& operator=(const Framebuffer& other) = delete;
        explicit Framebuffer(Framebuffer&& other) noexcept;
        Framebuffer& operator=(Framebuffer&& other) noexcept;

        const std::shared_ptr<Image> image;

    private:
        const std::shared_ptr<LogicalDevice> device;
        const std::shared_ptr<RenderPass> renderPass;

#ifdef GRAPHICS_API_GL

#elif defined(GRAPHICS_API_VULKAN)
    public:
        VkFramebuffer GetFramebuffer() const;

    private:
        VkFramebuffer framebuffer;
#endif
    };
}  // namespace UniverseEngine