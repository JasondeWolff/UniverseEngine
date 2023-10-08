#include "World.h"
#include "Engine.h"
#include "TerrianGenerator.h"
#include "Scene.h"

namespace UniverseEngine {
    World::World() : camera{}, sun{}, sceneInstances{} 
    {
        isTerrianGenerated = false;
        camera.transform.SetTranslation(glm::vec3(0,0,1));
        camera.transform.SetRotation(EulerToQuat(glm::vec3(180, 0, 0)));
    }

    void World::GenerateWorld() {
        TerrianGeneratorConfig config = { 25, 25, 25, 25, 1};

        tg.Init(config);
        isTerrianGenerated = true;

        if (tg.newlyGeneratedWorld.size() > 0) {
            for (auto sceneInstance : tg.newlyGeneratedWorld) {
                this->sceneInstances.push_back(sceneInstance);
                this->newInstances.push_back(sceneInstance);
            }
        }
        //tg.newlyGeneratedWorld.clear();
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
        if (isTerrianGenerated) {
            tg.Update();
        }
    }
}  // namespace UniverseEngine