#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

#include "../RenderPass.h"

namespace UniverseEngine {
    RenderPass::RenderPass(std::shared_ptr<LogicalDevice> device, GraphicsFormat colorFormat) : device(device) {
    }

    RenderPass::~RenderPass() {
    }
}  // namespace UniverseEngine
#endif