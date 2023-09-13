#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <memory>

#include "Transform.h"
#include "MeshRenderable.h"

namespace UniverseEngine {
    class Graphics;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
        glm::vec3 color;
    };

    struct MeshInstance {
        MeshInstance() : transform{}, meshIdx(0) {
        }
        MeshInstance(size_t idx) : transform{}, meshIdx(idx) {
        }

        Transform transform;
        size_t meshIdx;
    };

    struct Mesh {
        Mesh() = default;
        explicit Mesh(const Mesh& other) = delete;
        Mesh& operator=(const Mesh& other) = delete;
        explicit Mesh(Mesh&& other) noexcept = default;
        Mesh& operator=(Mesh&& other) noexcept = default;

        std::string name;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        size_t materialIdx;

    private:
        friend class Graphics;
        std::unique_ptr<MeshRenderable> renderable;

        void ClearCPUData();
    };
}  // namespace UniverseEngine