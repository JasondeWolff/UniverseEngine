#include "World.h"
#include "Engine.h"
#include "TerrianGenerator.h"

namespace UniverseEngine {
    World::World() : camera{}, sun{}, sceneInstances{} {
    }

    std::shared_ptr<SceneInstance> World::AddSceneInstance(std::shared_ptr<Scene> hScene) {
        auto sceneInstance = std::make_shared<SceneInstance>(hScene);
        this->sceneInstances.push_back(sceneInstance);
        this->newInstances.push_back(sceneInstance);
        return sceneInstance;
    }

    const std::vector<std::shared_ptr<SceneInstance>>& World::GetAllSceneInstances() const {
        return this->sceneInstances;
    }

    void World::Update() {
    }
}  // namespace UniverseEngine