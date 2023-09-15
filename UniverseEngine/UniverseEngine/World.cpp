#include "World.h"

namespace UniverseEngine {
    World::World() : camera{}, newInstances{} {
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

    std::vector<std::reference_wrapper<SceneInstance>> World::GetAllSceneInstances() {
        return this->sceneInstances->AllValues();
    }

    void World::Update() {
    }
}  // namespace UniverseEngine