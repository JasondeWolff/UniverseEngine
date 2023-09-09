#pragma once

#include <glm/vec4.hpp>

#include "Pool.h"
#include "Texture.h"

namespace UniverseEngine {
    struct Material {
        std::string name;

        glm::vec4 baseColor;
        Handle<Texture> baseColorMap;
    };
}  // namespace UniverseEngine