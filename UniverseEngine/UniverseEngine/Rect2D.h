#pragma once

#include <glm/vec2.hpp>

namespace UniverseEngine {
    struct Rect2D {
        Rect2D() = default;
        Rect2D(uint32_t width, uint32_t height) : offset{}, extent(width, height) {
        }

        glm::uvec2 offset;
        glm::uvec2 extent;
    };
}  // namespace UniverseEngine