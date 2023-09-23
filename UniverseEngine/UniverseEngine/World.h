#pragma once

#include <set>

#include "Resources.h"
#include "Camera.h"
#include "TerrianGenerator.h"

namespace UniverseEngine {
    class Engine;
    class Graphics;

    class World {
    public:
        Camera camera;

        std::shared_ptr<SceneInstance> AddSceneInstance(std::shared_ptr<Scene> hScene);
        const std::vector<std::shared_ptr<SceneInstance>>& GetAllSceneInstances() const;

    private:
        friend class Engine;
        friend class Graphics;
        World();

        void Update();

        TerrianGenerator tg;

        std::vector<std::shared_ptr<SceneInstance>> sceneInstances;
        std::vector<std::shared_ptr<SceneInstance>> newInstances;
    };
}  // namespace UniverseEngine