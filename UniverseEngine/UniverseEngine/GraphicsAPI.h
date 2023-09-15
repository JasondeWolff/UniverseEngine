#pragma once

#define GRAPHICS_API_GL

namespace UniverseEngine {
    class GraphicsAPI {
    public:
        static void Init();
        static const char* ShaderDir();
    };
}  // namespace UniverseEngine