#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

namespace UniverseEngine {
    std::unique_ptr<GraphicsInstance> GraphicsAPI::Init(const Window& window, bool debug) {
        return std::move(std::unique_ptr<GraphicsInstance>(new GraphicsInstance(window, debug)));
    }

    const char* GraphicsAPI::ShaderDir() {
        return "GL/";
    }
}  // namespace UniverseEngine
#endif