#pragma once

#include <memory>

#include "GraphicsInstance.h"

namespace UniverseEngine {
    class GraphicsAPI {
    public:
        static std::unique_ptr<GraphicsInstance> Init(bool debug);
        static const char* ShaderDir();
    };
}  // namespace UniverseEngine