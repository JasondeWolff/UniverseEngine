#pragma once

#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../DescriptorLayoutBinding.h"
#include "../GraphicsPipeline.h"

namespace UniverseEngine {
    VkDescriptorType GetVkDescriptorType(DescriptorType type);
    VkShaderStageFlags GetVkShaderStageFlags(GraphicsStageFlags stages);
    VkPipelineStageFlags GetVkPipelineStageFlags(PipelineStage stage);
}  // namespace UniverseEngine
#endif