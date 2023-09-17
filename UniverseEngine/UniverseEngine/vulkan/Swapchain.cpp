#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <algorithm>

#include "../Swapchain.h"
#include "../Logging.h"

namespace UniverseEngine {
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const GraphicsInstance& instance, const PhysicalDevice& physicalDevice) {
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice.GetPhysicalDevice(), instance.GetSurface(),
                                             &formatCount, nullptr);
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

        if (capabilities.currentExtent.width != UINT_MAX) {
            return capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = {window.Width(), window.Height()};

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                            capabilities.maxImageExtent.width);
            actualExtent.height =
                std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                           capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    uint32_t ChooseImageCount(const GraphicsInstance& instance,
                              const PhysicalDevice& physicalDevice) {
        VkSurfaceCapabilitiesKHR capabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice.GetPhysicalDevice(),
                                                  instance.GetSurface(), &capabilities);
        
        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 &&
            imageCount > capabilities.maxImageCount) {
            imageCount = capabilities.maxImageCount;
        }
        return imageCount;
    }

    Swapchain::Swapchain(const Window& window, const GraphicsInstance& instance, const LogicalDevice& device,
                         const PhysicalDevice& physicalDevice) : device(device) {
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
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = this->presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        UE_ASSERT_MSG(
            !vkCreateSwapchainKHR(device.GetDevice(), &createInfo, nullptr, &this->swapChain),
            "Failed to create swapchain.");
    }

    Swapchain::~Swapchain() {
        vkDestroySwapchainKHR(this->device.GetDevice(), this->swapChain, nullptr);
    }
}  // namespace UniverseEngine
#endif