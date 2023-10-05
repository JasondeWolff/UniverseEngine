#pragma once

#include <memory>
#include <vector>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class RenderPass;
    class LogicalDevice;
    class Image;
    class Swapchain;

    class Framebuffer {
    public:
        Framebuffer(std::shared_ptr<LogicalDevice> device,
                    std::vector<std::shared_ptr<Image>> images,
                    std::shared_ptr<RenderPass> renderPass);
        ~Framebuffer();

        explicit Framebuffer(const Framebuffer& other) = delete;
        Framebuffer& operator=(const Framebuffer& other) = delete;
        explicit Framebuffer(Framebuffer&& other) noexcept;
        Framebuffer& operator=(Framebuffer&& other) noexcept;

        const std::vector<std::shared_ptr<Image>>& Images() const;

    private:
        const std::shared_ptr<LogicalDevice> device;
        std::shared_ptr<RenderPass> renderPass;
        std::vector<std::shared_ptr<Image>> images;

#ifdef GRAPHICS_API_GL
    public:
        unsigned GetFramebuffer() const;

    private:
        friend class Swapchain;
        Framebuffer(std::shared_ptr<LogicalDevice> device,
                    std::vector<std::shared_ptr<Image>> images,
                    std::shared_ptr<RenderPass> renderPass, unsigned fbo);

        unsigned framebuffer;
#elif defined(GRAPHICS_API_VULKAN)
    public:
        VkFramebuffer GetFramebuffer() const;

    private:
        VkFramebuffer framebuffer;
#endif
    };
}  // namespace UniverseEngine