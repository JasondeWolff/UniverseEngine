#pragma once

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class Graphics;
    struct Shader;

    class ShaderRenderable {
    private:
        friend class Graphics;
        ShaderRenderable(const Shader& shader);
    };
}  // namespace UniverseEngine