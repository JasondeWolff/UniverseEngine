#pragma once

#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <vector>

namespace UniverseEngine {
    class VkPFN {
    public:
        static void Load(VkInstance instance);

        static PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
        static PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
    };
}  // namespace UniverseEngine
#endif