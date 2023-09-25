#pragma once

#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <vector>

namespace UniverseEngine {
    static const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    void CheckValidationLayerSupport();

    VkBool32 VulkanDebugCallback(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
                                 uint64_t srcObject, size_t location, int32_t msgCode,
                                 const char *pLayerPrefix, const char *pMsg, void *pUserData);
}  // namespace UniverseEngine
#endif