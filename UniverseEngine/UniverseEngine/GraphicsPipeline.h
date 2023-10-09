#pragma once

#include <memory>
#include <array>
#include <vector>
#include <bitset>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class ShaderRenderable;
    class LogicalDevice;
    class RenderPass;
    class DescriptorSetLayout;
    struct PushConstantRange;

    typedef std::bitset<13> PipelineStage;
    enum PipelineStageBits {
        PIPELINE_STAGE_TOP_OF_PIPE_BIT = 0x00000001,
        PIPELINE_STAGE_DRAW_INDIRECT_BIT = 0x00000002,
        PIPELINE_STAGE_VERTEX_INPUT_BIT = 0x00000004,
        PIPELINE_STAGE_VERTEX_SHADER_BIT = 0x00000008,
        PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT = 0x00000010,
        PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT = 0x00000020,
        PIPELINE_STAGE_GEOMETRY_SHADER_BIT = 0x00000040,
        PIPELINE_STAGE_FRAGMENT_SHADER_BIT = 0x00000080,
        PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT = 0x00000100,
        PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT = 0x00000200,
        PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT = 0x00000400,
        PIPELINE_STAGE_COMPUTE_SHADER_BIT = 0x00000800,
        PIPELINE_STAGE_TRANSFER_BIT = 0x00001000,
    };
    size_t GetPipelineStageBitIndex(PipelineStageBits bits);

    enum class PipelineType {
        GRAPHICS,
        COMPUTE
    };

    enum class GraphicsPolygonMode {
        FILL,
        LINE
    };

    struct GraphicsPipelineInfo {
        bool ignoreDepth = false;
        bool blending = false;
        bool inputVertices = true;
        GraphicsPolygonMode polygonMode = GraphicsPolygonMode::FILL;
        bool culling = true;
    };

    class GraphicsPipeline {
    public:
        GraphicsPipeline(std::shared_ptr<LogicalDevice> device,
                         const std::vector<const ShaderRenderable*>& shaders,
                         std::shared_ptr<RenderPass> renderPass,
                         std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts,
                         std::vector<PushConstantRange> pushConstants, GraphicsPipelineInfo info);
        ~GraphicsPipeline();
        GraphicsPipeline(const GraphicsPipeline& other) = delete;
        GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;

        const GraphicsPipelineInfo& Info() const;

    private:
        const std::shared_ptr<LogicalDevice> device;
        const std::shared_ptr<RenderPass> renderPass;
        const std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts;

        GraphicsPipelineInfo info;

#ifdef GRAPHICS_API_GL
    public:
        unsigned ShaderProgram() const;

    private:
        unsigned shaderProgram;
#elif defined(GRAPHICS_API_VULKAN)
    public:
        VkPipeline GetPipeline() const;
        VkPipelineLayout GetLayout() const;

    private:
        VkPipelineLayout pipelineLayout;
        VkPipeline pipeline;
#endif
    };
}  // namespace UniverseEngine