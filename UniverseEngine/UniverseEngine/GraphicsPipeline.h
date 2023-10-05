#pragma once

#include <memory>
#include <array>
#include <vector>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class ShaderRenderable;
    class LogicalDevice;
    class RenderPass;
    class DescriptorSetLayout;
    struct PushConstantRange;

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