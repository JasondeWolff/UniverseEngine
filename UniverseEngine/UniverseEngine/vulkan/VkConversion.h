#pragma once

#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../DescriptorLayoutBinding.h"

namespace UniverseEngine {
    VkDescriptorType GetVkDescriptorType(DescriptorType type);
    VkShaderStageFlags GetVkShaderStageFlags(GraphicsStageFlags stages);
}  // namespace UniverseEngine
#endif