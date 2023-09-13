#include "World.h"

namespace UniverseEngine {
    World::World() : newInstances{} {
        this->sceneInstances = std::make_unique<Pool<SceneInstance>>();
    }

    Handle<SceneInstance> World::AddSceneInstance(Handle<Scene> hScene) {
        Handle<SceneInstance> hSceneInstance = this->sceneInstances->Alloc();
        this->sceneInstances->Value(hSceneInstance).Value() = SceneInstance(hScene);
        this->newInstances.insert(hSceneInstance);
        return hSceneInstance;
    }

    void World::RemoveSceneInstance(Handle<SceneInstance> hSceneInstance) {
        this->sceneInstances->Free(hSceneInstance);
    }

    OptionalPtr<SceneInstance> World::GetSceneInstance(Handle<SceneInstance> hSceneInstance) {
        return this->sceneInstances->Value(hSceneInstance);
    }

    void World::Update() {
    }
}  // namespace UniverseEngine