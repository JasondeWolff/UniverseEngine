#pragma once

#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <vector>

#include "../PhysicalDevice.h"

namespace UniverseEngine {
    static const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    bool CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
}  // namespace UniverseEngine
#endif