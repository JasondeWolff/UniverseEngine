#pragma once

#include "Resources.h"

namespace UniverseEngine {
    class Engine;

    struct SceneInstance {
        SceneInstance() : transform{}, hScene{} {
        }
        SceneInstance(Handle<Scene> hScene) : transform{}, hScene(hScene) {
        }

        Transform transform;
        Handle<Scene> hScene;
    };

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