#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

namespace UniverseEngine {
    const char* GraphicsAPI::ShaderDir() {
        return "Vulkan/";
    }

    const char* GraphicsAPI::ShaderAppendix() {
        return ".spv";
    }
}  // namespace UniverseEngine
#endif