#pragma once

#include <memory>
#include <vector>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class ShaderRenderable;
    class LogicalDevice;
    class RenderPass;
    class DescriptorSetLayout;

    class GraphicsPipeline {
    public:
        GraphicsPipeline(std::shared_ptr<LogicalDevice> device,
                         const std::vector<ShaderRenderable*>& shaders,
                         std::shared_ptr<RenderPass> renderPass,
                         std::shared_ptr<DescriptorSetLayout> descriptorSetLayout);
        ~GraphicsPipeline();
        GraphicsPipeline(const GraphicsPipeline& other) = delete;
        GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;

    private:
        const std::shared_ptr<LogicalDevice> device;
        const std::shared_ptr<RenderPass> renderPass;
        const std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;

#ifdef GRAPHICS_API_GL
    public:
        unsigned ShaderProgram() const;

    private:
        unsigned shaderProgram;
#elif defined(GRAPHICS_API_VULKAN)
    public:
        VkPipeline GetPipeline() const;

    private:
        VkPipelineLayout pipelineLayout;
        VkPipeline pipeline;
#endif
    };
}  // namespace UniverseEngine