#include "../GraphicsAPI.h"
#ifdef GRAPHICS_API_GL

namespace UniverseEngine {
    const char* GraphicsAPI::ShaderDir() {
        return "GL/";
    }

    const char* GraphicsAPI::ShaderAppendix() {
        return "";
    }
}  // namespace UniverseEngine
#endif