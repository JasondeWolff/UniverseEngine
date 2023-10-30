#include "World.h"
#include "Engine.h"
#include "TerrianGenerator.h"
#include "Scene.h"

namespace UniverseEngine {
    World::World() : camera{}, sun{}, sceneInstances{} 
    {
        isTerrianGenerated = false;
        camera.transform.SetTranslation(glm::vec3(0,0,0));
        camera.transform.SetRotation(EulerToQuat(glm::vec3(180, 0, 0)));
    }

    void World::GenerateWorld(TerrianGeneratorConfig config) {
        float xOffset = (float)(config.chunkWidth * config.chunkRenderDistance) + config.chunkWidth;
        float zOffset = (float)(config.chunkHeight * config.chunkRenderDistance) + config.chunkHeight;
        xOffset = xOffset / 2;
        zOffset = zOffset / 2;

        auto& camera = Engine::GetWorld().camera;
        camera.transform.Translate(glm::vec3(xOffset, -5.0f, zOffset));

        tg.Init(config);
        isTerrianGenerated = true;
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