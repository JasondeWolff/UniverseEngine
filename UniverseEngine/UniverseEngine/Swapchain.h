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
#include "Semaphore.h"
#include "Fence.h"
#include "CmdQueue.h"

namespace UniverseEngine {
    class Swapchain {
    public:
        Swapchain(const Window& window, const GraphicsInstance& instance,
                  std::shared_ptr<LogicalDevice> device, const PhysicalDevice& physicalDevice);
        ~Swapchain();
        Swapchain(const Swapchain& other) = delete;
        Swapchain& operator=(const Swapchain& other) = delete;

        const Framebuffer& GetCurrentFramebuffer();
        const Image& GetCurrentImage();
        Semaphore& GetImageAvailableSemaphore();
        Semaphore& GetRenderFinishedSemaphore();

        void RebuildFramebuffers(std::shared_ptr<RenderPass> renderPass);
        std::shared_ptr<Fence> NextImage();
        void Present(const CmdQueue& queue, const Fence& fence, const std::vector<Semaphore*>& semaphores);

        GraphicsFormat Format() const;
        Rect2D Extent() const;

        const static uint32_t MAX_FRAMES_IN_FLIGHT = 2;

    private:
        const std::shared_ptr<LogicalDevice> device;

        uint32_t currentFrame;
        uint32_t currentImage;
        std::vector<Semaphore> imageAvailableSemaphores;
        std::vector<Semaphore> renderFinishedSemaphores;
        std::vector<std::shared_ptr<Fence>> inflightFences;

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
        std::vector<std::shared_ptr<Image>> images;
        std::vector<Framebuffer> framebuffers;
#endif
    };
}  // namespace UniverseEngine