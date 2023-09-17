#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

#include "../GraphicsPipeline.h"

#include "../Format.h"
#include "../Logging.h"
#include "../ShaderRenderable.h"

namespace UniverseEngine {
    GraphicsPipeline::GraphicsPipeline(const std::vector<ShaderRenderable*>& shaders) {
    }

    GraphicsPipeline::~GraphicsPipeline() {
    }
}  // namespace UniverseEngine
#endif