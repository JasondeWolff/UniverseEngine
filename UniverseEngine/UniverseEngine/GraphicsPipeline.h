#pragma once

#include <vector>

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class ShaderRenderable;

    class GraphicsPipeline {
    public:
        GraphicsPipeline(const std::vector<ShaderRenderable*>& shaders);
        ~GraphicsPipeline();

        GraphicsPipeline(const GraphicsPipeline& other) = delete;
        GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;

#ifdef GRAPHICS_API_GL
    public:
        unsigned ShaderProgram() const;
    private:
        unsigned shaderProgram;
#endif
    };
}  // namespace UniverseEngine