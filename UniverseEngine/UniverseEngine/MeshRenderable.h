#pragma once

namespace UniverseEngine {
    class Graphics;
    struct Mesh;

    class MeshRenderable {
    private:
        friend class Graphics;
        MeshRenderable(const Mesh& mesh);
    };
}