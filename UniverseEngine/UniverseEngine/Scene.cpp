#include "Scene.h"

namespace UniverseEngine {
    tree<MeshInstance> Scene::TransformedMeshHierarchy(const Transform& root) const {
        tree<MeshInstance> result = this->meshHierarchy;

        const glm::mat4& rootMatrix = root.GetMatrix();
        auto begin = result.begin();
        TransformedMeshHierarchyRec(result, rootMatrix, begin);

        return result;
    }

    void Scene::TransformedMeshHierarchyRec(tree<MeshInstance>& instances, glm::mat4 parentMatrix,
                                            tree<MeshInstance>::iterator_base parent) const {
        tree<MeshInstance>::sibling_iterator sibling = instances.begin(parent);
        while (sibling != instances.end(parent)) {
            auto& transform = sibling->transform;
            transform.SetMatrix(parentMatrix * transform.GetMatrix());
            TransformedMeshHierarchyRec(instances, transform.GetMatrix(), sibling);

            sibling++;
        }
    }
}  // namespace UniverseEngine