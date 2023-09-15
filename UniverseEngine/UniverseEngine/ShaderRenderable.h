#pragma once

#include "GraphicsAPI.h"
#include "ShaderType.h"

namespace UniverseEngine {
    class Graphics;
    struct Shader;

    class ShaderRenderable {
    public:
        ~ShaderRenderable();

    private:
        friend class Graphics;
        ShaderRenderable(const Shader& shader);

        ShaderType type;

#ifdef GRAPHICS_API_GL
    public:
        unsigned GetShader() const;
    private:
        unsigned shader;
#endif
    };
}  // namespace UniverseEngine