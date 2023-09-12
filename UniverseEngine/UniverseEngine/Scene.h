#pragma once

#include <memory>
#include <tree.hh>

#include "Material.h"
#include "Mesh.h"
#include "Pool.h"

namespace UniverseEngine {
    struct Scene;

    struct SceneInstance {
        SceneInstance() : transform{}, hScene{} {
        }
        SceneInstance(Handle<Scene> hScene) : transform{}, hScene(hScene) {
        }

        Transform transform;
        Handle<Scene> hScene;
    };

    struct Scene {
        std::string name;

        tree<MeshInstance> instanceTree;
        std::vector<Mesh> meshes;
        std::vector<Material> materials;
    };
}  // namespace UniverseEngine