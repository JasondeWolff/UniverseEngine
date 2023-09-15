#pragma once

#include <set>

#include "Resources.h"
#include "Camera.h"

namespace UniverseEngine {
    class Engine;
    class Graphics;

    class World {
    public:
        Camera camera;

        Handle<SceneInstance> AddSceneInstance(Handle<Scene> hScene);
        void RemoveSceneInstance(Handle<SceneInstance> hSceneInstance);
        OptionalPtr<SceneInstance> GetSceneInstance(Handle<SceneInstance> hSceneInstance);

    private:
        friend class Engine;
        friend class Graphics;
        World();

        void Update();

        std::unique_ptr<Pool<SceneInstance>> sceneInstances;
        std::set<Handle<SceneInstance>> newInstances;
    };
}  // namespace UniverseEngine