#pragma once

#include <glm/vec4.hpp>
#include <optional>

#include "AtomicPool.h"
#include "Texture.h"

namespace UniverseEngine {
    struct Material {
        std::string name;

        glm::vec4 baseColor;
        std::optional<AtomicHandle<Texture>> baseColorMap;
    };
}  // namespace UniverseEngine