#pragma once

#include <vector>

#include "GraphicsAPI.h"

#include "GraphicsInstance.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "Window.h"

namespace UniverseEngine {
    class Swapchain {
    public:
        Swapchain(const Window& window, const GraphicsInstance& instance, const LogicalDevice& device,
                  const PhysicalDevice& physicalDevice);
        ~Swapchain();
        Swapchain(const Swapchain& other) = delete;
        Swapchain& operator=(const Swapchain& other) = delete;

#ifdef GRAPHICS_API_VULKAN
    private:
        const LogicalDevice& device;

        VkSwapchainKHR swapChain;
        VkSurfaceFormatKHR format;
        VkPresentModeKHR presentMode;
        VkExtent2D extent;

        uint32_t imageCount;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
#endif
    };
}  // namespace UniverseEngine