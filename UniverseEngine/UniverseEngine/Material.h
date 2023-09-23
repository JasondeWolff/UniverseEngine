#pragma once

#include <glm/vec4.hpp>

#include "Texture.h"

namespace UniverseEngine {
    struct Material {
        std::string name;

        glm::vec4 baseColor;
        std::shared_ptr<Texture> baseColorMap;
    };
}  // namespace UniverseEngine