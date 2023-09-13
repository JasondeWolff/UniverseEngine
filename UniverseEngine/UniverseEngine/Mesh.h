#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <optional>

#include "Transform.h"
#include "MeshRenderable.h"

namespace UniverseEngine {
    class Graphics;

    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
        glm::vec3 color;
    };

    struct MeshInstance {
        MeshInstance() : transform{}, meshIdx(0) {
        }
        MeshInstance(int idx) : transform{}, meshIdx(idx) {
        }

        Transform transform;
        size_t meshIdx;
    };

    struct Mesh {
        std::string name;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        size_t materialIdx;

    private:
        friend class Graphics;
        std::optional<MeshRenderable> renderable;

        void ClearCPUData();
    };
}  // namespace UniverseEngine