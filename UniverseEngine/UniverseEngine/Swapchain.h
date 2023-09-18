#pragma once

#include <vector>

#include "GraphicsAPI.h"

#include "GraphicsInstance.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Window.h"
#include "Rect2D.h"
#include "GraphicsFormat.h"
#include "RenderPass.h"

namespace UniverseEngine {
    class Swapchain {
    public:
        Swapchain(const Window& window, const GraphicsInstance& instance, std::shared_ptr<LogicalDevice> device,
                  const PhysicalDevice& physicalDevice);
        ~Swapchain();
        Swapchain(const Swapchain& other) = delete;
        Swapchain& operator=(const Swapchain& other) = delete;

        void RebuildFramebuffers(const RenderPass& renderPass);

        GraphicsFormat Format() const;
        Rect2D Extent() const;

#ifdef GRAPHICS_API_GL
        uint32_t width, height;
#elif defined(GRAPHICS_API_VULKAN)
    private:
        const std::shared_ptr<LogicalDevice> device;

        VkSwapchainKHR swapChain;
        VkSurfaceFormatKHR format;
        VkPresentModeKHR presentMode;
        VkExtent2D extent;

        uint32_t imageCount;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
#endif
    };
}  // namespace UniverseEngine