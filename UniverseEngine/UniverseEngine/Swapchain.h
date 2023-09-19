#pragma once

#include <memory>
#include <vector>

#include "Framebuffer.h"
#include "GraphicsAPI.h"
#include "GraphicsFormat.h"
#include "GraphicsInstance.h"
#include "Image.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Rect2D.h"
#include "RenderPass.h"
#include "Window.h"

namespace UniverseEngine {
    class Swapchain {
    public:
        Swapchain(const Window& window, const GraphicsInstance& instance,
                  std::shared_ptr<LogicalDevice> device, const PhysicalDevice& physicalDevice);
        ~Swapchain();
        Swapchain(const Swapchain& other) = delete;
        Swapchain& operator=(const Swapchain& other) = delete;

        const Framebuffer& GetCurrentFramebuffer();
        void RebuildFramebuffers(std::shared_ptr<RenderPass> renderPass);

        GraphicsFormat Format() const;
        Rect2D Extent() const;

    private:
        const std::shared_ptr<LogicalDevice> device;

#ifdef GRAPHICS_API_GL
    private:
        uint32_t width, height;
        std::unique_ptr<Framebuffer> framebuffer;
#elif defined(GRAPHICS_API_VULKAN)
    private:
        VkSwapchainKHR swapChain;
        VkSurfaceFormatKHR format;
        VkPresentModeKHR presentMode;
        VkExtent2D extent;

        uint32_t imageCount;
        std::vector<std::shared_ptr<Image> > images;
        std::vector<Framebuffer> framebuffers;
#endif
    };
}  // namespace UniverseEngine