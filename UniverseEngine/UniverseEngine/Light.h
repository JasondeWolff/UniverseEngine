#pragma once

#include <glm/vec3.hpp>

namespace UniverseEngine {
    struct PointLight {
        glm::vec3 color;
        float intensity;
    };
}  // namespace UniverseEngine