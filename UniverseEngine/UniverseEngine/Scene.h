#pragma once

#include <memory>
#include <tree.hh>

#include "Material.h"
#include "Mesh.h"
#include "Light.h"

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

    struct SceneNode {
        SceneNode() = default;

        Transform transform{};
        std::optional<size_t> meshIdx = std::nullopt;
        std::optional<size_t> pointLightIdx = std::nullopt;
    };

    struct Scene {
        Scene() = default;
        explicit Scene(const Scene& other) = delete;
        Scene& operator=(const Scene& other) = delete;
        explicit Scene(Scene&& other) noexcept = default;
        Scene& operator=(Scene&& other) noexcept = default;

        std::string name;
        tree<SceneNode> hierarchy;

        std::vector<LODMesh> meshes;
        std::vector<Material> materials;
        std::vector<PointLight> pointLights;

        void BuildLODS();
        tree<SceneNode> TransformedHierarchy(const Transform& root) const;

    private:
        void TransformedHierarchyRec(tree<SceneNode>& instances, glm::mat4 parentMatrix,
                                         tree<SceneNode>::iterator_base parent) const;
    };
}  // namespace UniverseEngine