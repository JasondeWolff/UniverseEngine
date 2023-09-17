#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_VULKAN

namespace UniverseEngine {
    const char* GraphicsAPI::ShaderDir() {
        return "GL/";
    }
}  // namespace UniverseEngine
#endif