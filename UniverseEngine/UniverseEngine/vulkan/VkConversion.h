#pragma once

#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../DescriptorLayoutBinding.h"

namespace UniverseEngine {
    VkDescriptorType GetVkDescriptorType(DescriptorType type);
    VkShaderStageFlags GetVkShaderStageFlags(DescriptorStageFlags stages);
}  // namespace UniverseEngine
#endif