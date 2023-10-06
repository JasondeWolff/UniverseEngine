#include "VkConversion.h"
#ifdef GRAPHICS_API_VULKAN

namespace UniverseEngine {
    VkDescriptorType GetVkDescriptorType(DescriptorType type) {
        switch (type) {
            case DescriptorType::SAMPLER:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            case DescriptorType::COMBINED_IMAGE_SAMPLER:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            case DescriptorType::SAMPLED_IMAGE:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            case DescriptorType::STORAGE_IMAGE:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            case DescriptorType::UNIFORM_BUFFER:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case DescriptorType::STORAGE_BUFFER:
                return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        }
        return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
    }

    VkShaderStageFlags GetVkShaderStageFlags(GraphicsStageFlags stages) {
        VkShaderStageFlags flags = 0;

        if (stages.test(GetGraphicsStageBitIndex(GraphicsStageFlagBits::VERTEX_STAGE))) {
            flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
        }
        if (stages.test(GetGraphicsStageBitIndex(GraphicsStageFlagBits::FRAGMENT_STAGE))) {
            flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        if (stages.test(GetGraphicsStageBitIndex(GraphicsStageFlagBits::COMPUTE_STAGE))) {
            flags |= VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
        }

        return flags;
    }

    VkPipelineStageFlags GetVkPipelineStageFlags(PipelineStage stage) {
        VkAccessFlags flags = 0;

        if (stage.test(
                GetPipelineStageBitIndex(PipelineStageBits::PIPELINE_STAGE_TOP_OF_PIPE_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        }
        if (stage.test(
                GetPipelineStageBitIndex(PipelineStageBits::PIPELINE_STAGE_DRAW_INDIRECT_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
        }
        if (stage.test(
                GetPipelineStageBitIndex(PipelineStageBits::PIPELINE_STAGE_VERTEX_INPUT_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
        }
        if (stage.test(
                GetPipelineStageBitIndex(PipelineStageBits::PIPELINE_STAGE_VERTEX_SHADER_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
        }
        if (stage.test(GetPipelineStageBitIndex(
                PipelineStageBits::PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
        }
        if (stage.test(GetPipelineStageBitIndex(
                PipelineStageBits::PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
        }
        if (stage.test(
                GetPipelineStageBitIndex(PipelineStageBits::PIPELINE_STAGE_GEOMETRY_SHADER_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
        }
        if (stage.test(
                GetPipelineStageBitIndex(PipelineStageBits::PIPELINE_STAGE_FRAGMENT_SHADER_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        if (stage.test(GetPipelineStageBitIndex(
                PipelineStageBits::PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        if (stage.test(GetPipelineStageBitIndex(
                PipelineStageBits::PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        }
        if (stage.test(GetPipelineStageBitIndex(
                PipelineStageBits::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }
        if (stage.test(
                GetPipelineStageBitIndex(PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
        }
        if (stage.test(GetPipelineStageBitIndex(PipelineStageBits::PIPELINE_STAGE_TRANSFER_BIT))) {
            flags |= VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT;
        }

        return flags;
    }
}  // namespace UniverseEngine
#endif