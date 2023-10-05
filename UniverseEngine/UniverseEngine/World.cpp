#include "World.h"
#include "Engine.h"
#include "TerrianGenerator.h"

namespace UniverseEngine {
    World::World() : camera{}, sun{}, sceneInstances{} 
    {
        camera.transform.SetTranslation(glm::vec3(0,0,1));
        camera.transform.SetRotation(EulerToQuat(glm::vec3(180, 0, 0)));
    }

    void World::GenerateWorld() {

        //Generate a 1x1 grid
        tg.Init(25,25,25,25);
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
        tg.Update();
        
        for (int x = 0; x < tg.newlyGeneratedWorld.size(); x++) {
            for (int y = 0; y < tg.newlyGeneratedWorld[x].size(); y++) {
                auto sceneInstance = std::make_shared<SceneInstance>(tg.newlyGeneratedWorld[x][y]);
                this->sceneInstances.push_back(sceneInstance);
                this->newInstances.push_back(sceneInstance);
            }
        }

        //After we update the world with all the new chunks we want to clear the array
        //  so we dont keep on adding the same mesh
        for (auto& innerVector : tg.newlyGeneratedWorld) {
            innerVector.clear();
        }
        tg.newlyGeneratedWorld.clear();
    }
}  // namespace UniverseEngine