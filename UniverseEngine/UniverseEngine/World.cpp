#include "World.h"
#include "Engine.h"
#include "TerrianGenerator.h"

namespace UniverseEngine {
    World::World() : camera{}, sun{}, sceneInstances{} 
    {
        camera.transform.SetTranslation(glm::vec3(0,0,1));
        camera.transform.SetRotation(EulerToQuat(glm::vec3(180, 0, 0)));
    }

    std::shared_ptr<SceneInstance> World::GenerateWorld() {

        //Generate a 1x1 grid
        tg.Init();

        auto sceneInstance = std::make_shared<SceneInstance>(tg.generatedWorld.at(0).at(0));
        this->sceneInstances.push_back(sceneInstance);
        this->newInstances.push_back(sceneInstance);
        return sceneInstance;
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