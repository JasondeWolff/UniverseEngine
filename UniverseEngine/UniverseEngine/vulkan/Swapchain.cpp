#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <algorithm>

#include "../Format.h"
#include "../Logging.h"
#include "../Swapchain.h"
#include "VkGraphicsFormat.h"

namespace UniverseEngine {
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const GraphicsInstance& instance,
                                               const PhysicalDevice& physicalDevice) {
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.GetPhysicalDevice(),
                                             instance.GetSurface(), &formatCount, nullptr);
        UE_ASSERT_MSG(formatCount > 0, "No swapchain formats found.");

        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.GetPhysicalDevice(),
                                             instance.GetSurface(), &formatCount, formats.data());

        for (const auto& availableFormat : formats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return formats[0];
    }

    VkPresentModeKHR ChooseSwapPresentMode(const GraphicsInstance& instance,
                                           const PhysicalDevice& physicalDevice) {
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevice.GetPhysicalDevice(), instance.GetSurface(), &presentModeCount, nullptr);
        UE_ASSERT_MSG(presentModeCount > 0, "No present modes found.");

        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice.GetPhysicalDevice(),
                                                  instance.GetSurface(), &presentModeCount,
                                                  presentModes.data());

        for (const auto& availablePresentMode : presentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D ChooseSwapExtent(const Window& window, const GraphicsInstance& instance,
                                const PhysicalDevice& physicalDevice) {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.GetPhysicalDevice(),
                                                  instance.GetSurface(), &capabilities);

        VkExtent2D extent;
        if (capabilities.currentExtent.width != UINT_MAX) {
            extent = capabilities.currentExtent;
        } else {
            extent = {window.Width(), window.Height()};

            extent.width = std::clamp(extent.width, capabilities.minImageExtent.width,
                                            capabilities.maxImageExtent.width);
            extent.height = std::clamp(extent.height, capabilities.minImageExtent.height,
                           capabilities.maxImageExtent.height);
        }

        extent.width = std::max(2U, extent.width);
        extent.height = std::max(2U, extent.height);

        return extent;
    }

    uint32_t ChooseImageCount(const GraphicsInstance& instance,
                              const PhysicalDevice& physicalDevice) {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.GetPhysicalDevice(),
                                                  instance.GetSurface(), &capabilities);

        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
            imageCount = capabilities.maxImageCount;
        }
        return imageCount;
    }

    Swapchain::Swapchain(const Window& window, const GraphicsInstance& instance,
                         std::shared_ptr<LogicalDevice> device,
                         const PhysicalDevice& physicalDevice)
        : device(device), currentFrame(0), currentImage(0) {
        this->format = ChooseSwapSurfaceFormat(instance, physicalDevice);
        this->presentMode = ChooseSwapPresentMode(instance, physicalDevice);
        this->extent = ChooseSwapExtent(window, instance, physicalDevice);
        this->imageCount = ChooseImageCount(instance, physicalDevice);

        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.GetPhysicalDevice(),
                                                  instance.GetSurface(), &capabilities);

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = instance.GetSurface();
        createInfo.minImageCount = this->imageCount;
        createInfo.imageFormat = this->format.format;
        createInfo.imageColorSpace = this->format.colorSpace;
        createInfo.imageExtent = this->extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = this->presentMode;
        createInfo.clipped = VK_TRUE;
        
        if (physicalDevice.GraphicsFamily() != physicalDevice.PresentFamily()) {
            uint32_t queueFamilyIndices[] = {physicalDevice.GraphicsFamily(),
                                             physicalDevice.PresentFamily()};

            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        UE_ASSERT_MSG(
            !vkCreateSwapchainKHR(device->GetDevice(), &createInfo, nullptr, &this->swapChain),
            "Failed to create swapchain.");

        vkGetSwapchainImagesKHR(device->GetDevice(), this->swapChain, &this->imageCount, nullptr);
        std::vector<VkImage> swapChainImages(this->imageCount);
        vkGetSwapchainImagesKHR(device->GetDevice(), this->swapChain, &this->imageCount,
                                swapChainImages.data());

        std::vector<VkImageView> swapChainImageViews(swapChainImages.size());
        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = this->format.format;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            UE_ASSERT_MSG(!vkCreateImageView(device->GetDevice(), &createInfo, nullptr,
                                             &swapChainImageViews[i]),
                          "Failed to create image view.");
        }

        this->images.reserve(swapChainImages.size());
        for (size_t i = 0; i < swapChainImages.size(); i++) {
            this->images.push_back(
                std::make_shared<Image>(device, swapChainImages[i], swapChainImageViews[i],
                                        this->extent.width, this->extent.height));
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            imageAvailableSemaphores.emplace_back(
                std::move(Semaphore(UniverseEngine::Format("Image Available %i", i), device)));
            renderFinishedSemaphores.emplace_back(
                std::move(Semaphore(UniverseEngine::Format("Render Finished %i", i), device)));
            inflightFences.emplace_back(std::move(std::make_shared<Fence>(device, true)));
        }
    }

    Swapchain::~Swapchain() {
        vkDestroySwapchainKHR(this->device->GetDevice(), this->swapChain, nullptr);
    }

    const Framebuffer& Swapchain::GetCurrentFramebuffer() const {
        UE_ASSERT_MSG(!this->framebuffers.empty(), "Rebuild framebuffers first!");
        return this->framebuffers[static_cast<size_t>(this->currentImage)];
    }

    const Image& Swapchain::GetCurrentImage() const {
        return *this->images[static_cast<size_t>(this->currentImage)];
    }

    uint32_t Swapchain::GetCurrentFrameIdx() const {
        return this->currentFrame;
    }

    Semaphore& Swapchain::GetImageAvailableSemaphore() {
        return this->imageAvailableSemaphores[static_cast<size_t>(this->currentFrame)];
    }

    Semaphore& Swapchain::GetRenderFinishedSemaphore() {
        return this->renderFinishedSemaphores[static_cast<size_t>(this->currentFrame)];
    }

    void Swapchain::RebuildFramebuffers(std::shared_ptr<RenderPass> renderPass,
                                        std::shared_ptr<Image> depthImage) {
        this->framebuffers.clear();
        for (auto& image : this->images) {
            std::vector<std::shared_ptr<Image>> images{image};
            if (depthImage) {
                images.push_back(depthImage);
            }

            this->framebuffers.emplace_back(
                std::move(Framebuffer(this->device, images, renderPass)));
        }
    }

    std::shared_ptr<Fence> Swapchain::NextImage() {
        this->inflightFences[this->currentFrame]->Wait();
        this->inflightFences[this->currentFrame]->Reset();

        vkAcquireNextImageKHR(
            this->device->GetDevice(), this->swapChain, std::numeric_limits<uint64_t>::max(),
            this->GetImageAvailableSemaphore().GetSemaphore(), VK_NULL_HANDLE, &this->currentImage);

        return this->inflightFences[this->currentFrame];
    }

    void Swapchain::Present(const CmdQueue& queue, const Fence& fence,
                            const std::vector<Semaphore*>& semaphores) {
        std::vector<VkSemaphore> vkSemaphores;
        for (auto& semaphore : semaphores) {
            vkSemaphores.push_back(semaphore->GetSemaphore());
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = static_cast<uint32_t>(vkSemaphores.size());
        presentInfo.pWaitSemaphores = vkSemaphores.data();
        VkSwapchainKHR swapChains[] = {this->swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &this->currentImage;

        vkQueuePresentKHR(queue.GetQueue(), &presentInfo);

        this->currentFrame = (this->currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    GraphicsFormat Swapchain::Format() const {
        return VkGraphicsFormat::From(this->format.format);
    }

    Rect2D Swapchain::Extent() const {
        return Rect2D(this->extent.width, this->extent.height);
    }
}  // namespace UniverseEngine
#endif