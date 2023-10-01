#pragma once

#include <glm/vec2.hpp>

namespace UniverseEngine {
    struct Rect2D {
        Rect2D() = default;
        Rect2D(float width, float height) : offset{}, extent(width, height) {
        }

        glm::vec2 offset;
        glm::vec2 extent;
    };
}  // namespace UniverseEngine