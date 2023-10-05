#pragma once

#include <array>
#include <memory>
#include <vector>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class ShaderRenderable;
    class LogicalDevice;
    class RenderPass;
    class DescriptorSetLayout;
    struct PushConstantRange;

    class ComputePipeline {
    public:
        ComputePipeline(
            std::shared_ptr<LogicalDevice> device, const ShaderRenderable& shader,
                         std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts);
        ~ComputePipeline();
        ComputePipeline(const ComputePipeline& other) = delete;
        ComputePipeline& operator=(const ComputePipeline& other) = delete;

    private:
        const std::shared_ptr<LogicalDevice> device;
        const std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts;

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