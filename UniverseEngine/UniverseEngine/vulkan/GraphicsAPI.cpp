#include "../GraphicsAPI.h"

namespace UniverseEngine {
    std::unique_ptr<GraphicsInstance> GraphicsAPI::Init(bool debug) {
        return std::move(std::unique_ptr<GraphicsInstance>(new GraphicsInstance(debug)));
    }

    const char* GraphicsAPI::ShaderDir() {
        return "GL/";
    }
}  // namespace UniverseEngine