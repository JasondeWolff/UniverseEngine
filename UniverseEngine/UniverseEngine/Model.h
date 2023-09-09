#pragma once

#include "Material.h"
#include "Mesh.h"

namespace UniverseEngine {
    struct Model {
        std::string name;

        std::vector<Mesh> meshes;
        std::vector<Material> materials;
    };
}  // namespace UniverseEngine