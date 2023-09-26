#pragma once

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

#include "Texture.h"

namespace UniverseEngine {
    struct Material {
        std::string name;

        glm::vec4 baseColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
        float metallicFactor = 0.0f;
        float roughnessFactor = 1.0f;
        glm::vec3 emissiveFactor = glm::vec3(0.0, 0.0, 0.0);
        float normalFactor = 1.0f;
        float occlusionFactor = 0.0f;

        std::shared_ptr<Texture> baseColorMap = nullptr;
        std::shared_ptr<Texture> metallicRoughnessMap = nullptr;
        std::shared_ptr<Texture> emissiveMap = nullptr;
        std::shared_ptr<Texture> normalMap = nullptr;
        std::shared_ptr<Texture> occlusionMap = nullptr;
    };
}  // namespace UniverseEngine