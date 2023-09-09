#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace UniverseEngine {
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
        glm::vec3 color;
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        size_t materialIdx;
    };
}  // namespace UniverseEngine