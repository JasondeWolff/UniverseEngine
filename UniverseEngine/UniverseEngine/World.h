#pragma once

#include "Resources.h"

namespace UniverseEngine {
    class Engine;

    class World {
    public:
        Handle<SceneInstance> AddSceneInstance(Handle<Scene> hScene);
        void RemoveSceneInstance(Handle<SceneInstance> hSceneInstance);

        OptionalPtr<SceneInstance> GetSceneInstance(Handle<SceneInstance> hSceneInstance);

    private:
        friend class Engine;
        World();

        std::unique_ptr<Pool<SceneInstance>> sceneInstances;
    };
}  // namespace UniverseEngine