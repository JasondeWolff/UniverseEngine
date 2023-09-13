#pragma once

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class Graphics;
    struct Shader;

    class ShaderRenderable {
    private:
        friend class Graphics;
        ShaderRenderable(const Shader& shader);

#ifdef GRAPHICS_API_GL
#endif
    };
}  // namespace UniverseEngine