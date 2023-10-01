#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>

#include "MaterialRenderable.h"
#include "Texture.h"

namespace UniverseEngine {
    class Graphics;
    class TerrianGenerator;

    struct Material {
        Material() = default;
        explicit Material(const Material& other) = delete;
        Material& operator=(const Material& other) = delete;
        explicit Material(Material&& other) noexcept = default;
        Material& operator=(Material&& other) noexcept = default;

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

    private:
        friend class Graphics;
        std::unique_ptr<MaterialRenderable> renderable;
    };
}  // namespace UniverseEngine