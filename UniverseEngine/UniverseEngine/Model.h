#pragma once

#include <tree.hh>

#include "Material.h"
#include "Mesh.h"

namespace UniverseEngine {
    struct Model {
        std::string name;

        tree<MeshInstance> instanceTree;
        std::vector<Mesh> meshes;
        std::vector<Material> materials;
    };
}  // namespace UniverseEngine
