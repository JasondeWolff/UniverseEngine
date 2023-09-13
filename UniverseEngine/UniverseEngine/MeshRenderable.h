#pragma once

#include "GraphicsAPI.h"

namespace UniverseEngine {
    class Graphics;
    struct Mesh;

    class MeshRenderable {
    private:
        friend class Graphics;
        MeshRenderable(const Mesh& mesh);

        void Draw();

        size_t indexCount;

#ifdef GRAPHICS_API_GL
        unsigned vao;
        unsigned vbo;
        unsigned ebo;
#endif
    };
}  // namespace UniverseEngine