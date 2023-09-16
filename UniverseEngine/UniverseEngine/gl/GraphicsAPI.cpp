#include "../GraphicsAPI.h"

namespace UniverseEngine {
    std::unique_ptr<GraphicsInstance> GraphicsAPI::Init() {
        return std::move(std::unique_ptr<GraphicsInstance>(new GraphicsInstance{}));
    }

    const char* GraphicsAPI::ShaderDir() {
        return "GL/";
    }
}  // namespace UniverseEngine