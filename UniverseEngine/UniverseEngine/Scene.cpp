#include "Scene.h"

namespace UniverseEngine {
    tree<MeshInstance> Scene::TransformedMeshHierarchy(const Transform& root) const {
        tree<MeshInstance> result = this->meshHierarchy;

        const glm::mat4& rootMatrix = root.GetMatrix();

        for (auto& instance : result) {
            auto& transform = instance.transform;
            transform.SetMatrix(transform.GetMatrix() * rootMatrix);
        }

        return result;
    }
}