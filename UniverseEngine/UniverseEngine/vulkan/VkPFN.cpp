#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "VkPFN.h"

namespace UniverseEngine {
    PFN_vkSetDebugUtilsObjectNameEXT VkPFN::vkSetDebugUtilsObjectNameEXT = VK_NULL_HANDLE;
    PFN_vkCreateDebugReportCallbackEXT VkPFN::vkCreateDebugReportCallbackEXT = VK_NULL_HANDLE;

    void VkPFN::Load(VkInstance instance) {
        VkPFN::vkSetDebugUtilsObjectNameEXT =
            (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(instance,
                                                                    "vkSetDebugUtilsObjectNameEXT");
        VkPFN::vkCreateDebugReportCallbackEXT =
            (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
                instance, "vkCreateDebugReportCallbackEXT");
    }
}  // namespace UniverseEngine

#endif