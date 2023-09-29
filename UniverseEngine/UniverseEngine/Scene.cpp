#include "Scene.h"

namespace UniverseEngine {
    tree<SceneNode> Scene::TransformedHierarchy(const Transform& root) const {
        tree<SceneNode> result = this->hierarchy;

        const glm::mat4& rootMatrix = root.GetMatrix();
        auto begin = result.begin();
        TransformedHierarchyRec(result, rootMatrix, begin);

        return result;
    }

    void Scene::TransformedHierarchyRec(tree<SceneNode>& instances, glm::mat4 parentMatrix,
                                            tree<SceneNode>::iterator_base parent) const {
        tree<SceneNode>::sibling_iterator sibling = instances.begin(parent);
        while (sibling != instances.end(parent)) {
            auto& transform = sibling->transform;
            transform.SetMatrix(parentMatrix * transform.GetMatrix());
            TransformedHierarchyRec(instances, transform.GetMatrix(), sibling);

            sibling++;
        }
    }
}  // namespace UniverseEngine