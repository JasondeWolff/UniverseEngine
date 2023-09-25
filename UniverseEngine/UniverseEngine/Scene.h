#pragma once

#include <memory>
#include <tree.hh>

#include "Material.h"
#include "Mesh.h"

namespace UniverseEngine {
    struct Scene;

    struct SceneInstance {
        SceneInstance() : transform{}, hScene{} {
        }
        SceneInstance(std::shared_ptr<Scene> hScene) : transform{}, hScene(hScene) {
        }

        Transform transform;
        std::shared_ptr<Scene> hScene;
    };

    struct Scene {
        Scene() = default;
        explicit Scene(const Scene& other) = delete;
        Scene& operator=(const Scene& other) = delete;
        explicit Scene(Scene&& other) noexcept = default;
        Scene& operator=(Scene&& other) noexcept = default;

        std::string name;

        tree<MeshInstance> meshHierarchy;
        std::vector<Mesh> meshes;
        std::vector<Material> materials;

        tree<MeshInstance> TransformedMeshHierarchy(const Transform& root) const;

    private:
        void TransformedMeshHierarchyRec(tree<MeshInstance>& instances, glm::mat4 parentMatrix,
                                         tree<MeshInstance>::iterator_base parent) const;
    };
}  // namespace UniverseEngine