#pragma once

#include <vector>
#include <memory>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class ShaderRenderable;
    class LogicalDevice;

    class GraphicsPipeline {
    public:
        GraphicsPipeline(
            std::shared_ptr<LogicalDevice> device, const std::vector<ShaderRenderable*>& shaders);
        ~GraphicsPipeline();
        GraphicsPipeline(const GraphicsPipeline& other) = delete;
        GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;

        const std::shared_ptr<LogicalDevice> device;

#ifdef GRAPHICS_API_GL
    public:
        unsigned ShaderProgram() const;

    private:
        unsigned shaderProgram;
#elif defined(GRAPHICS_API_VULKAN)
    private:
        VkPipelineLayout pipelineLayout;
        VkPipeline pipeline;
#endif
    };
}  // namespace UniverseEngine