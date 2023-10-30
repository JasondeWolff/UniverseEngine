#pragma once

#include <set>

#include "Resources.h"
#include "Camera.h"
#include "TerrianGenerator.h"

namespace UniverseEngine {
    class Engine;
    class Graphics;

    struct Sun {
        glm::vec3 direction = glm::vec3(0.3, -1.0, 0.0);
        DirectionalLight lightSource{};
    };

    class World {
    public:
        Camera camera;
        Sun sun;

        void GenerateWorld(TerrianGeneratorConfig config);
        std::shared_ptr<SceneInstance> AddSceneInstance(std::shared_ptr<Scene> hScene);
        const std::vector<std::shared_ptr<SceneInstance>>& GetAllSceneInstances() const;
        
    private:
        friend class Engine;
        friend class Graphics;
        World();

        void Update();

        bool isTerrianGenerated;
        TerrianGenerator tg;

        std::vector<std::shared_ptr<SceneInstance>> sceneInstances;
        std::vector<std::shared_ptr<SceneInstance>> newInstances;
    };
}  // namespace UniverseEngine