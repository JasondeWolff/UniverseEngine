#pragma once

#include <glm/vec3.hpp>

namespace UniverseEngine {
    struct PointLight {
        glm::vec3 color = glm::vec3(1.0);
        float intensity = 500.0;
    };

    struct DirectionalLight {
        glm::vec3 color = glm::vec3(1.0);
        float intensity = 1.0;
    };
}  // namespace UniverseEngine