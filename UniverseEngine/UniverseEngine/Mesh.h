#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <memory>
#include <optional>

#include "Transform.h"
#include "MeshRenderable.h"

namespace UniverseEngine {
    class Graphics;

    struct Vertex {
        glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 normal = glm::vec3(0.0, 1.0, 0.0);
        glm::vec2 texCoord = glm::vec2(0.0, 0.0);
        glm::vec4 tangent = glm::vec4(0.0, 0.0, 0.0, 1.0);
        glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);
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

        bool HasTangents() const;
        void GenerateTangents();

        Mesh BuildSimplified(float percentage) const;

    private:
        friend class Graphics;
        std::unique_ptr<MeshRenderable> renderable;

        void ClearCPUData();
    };

    struct LODMesh {
        LODMesh(Mesh&& mesh);

        std::optional<size_t> BestLOD(const glm::vec3& observer, const glm::vec3& mesh) const;

        struct Config {
            float detail = 1.0f;
            float maxDistance = 5.0f;
        };

        std::vector<Mesh> lods;
        std::vector<Config> configs;
    };
}  // namespace UniverseEngine