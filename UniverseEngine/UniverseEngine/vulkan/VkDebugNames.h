#pragma once

#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include <string>

#include "../LogicalDevice.h"

namespace UniverseEngine {
    class VkDebugNames {
    public:
        static void Set(const LogicalDevice& device, VkObjectType type, uint64_t handle, const std::string& name);
    };
}  // namespace UniverseEngine
#endif