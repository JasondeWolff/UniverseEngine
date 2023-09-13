#include "Graphics.h"

#include "Engine.h"

namespace UniverseEngine {
    Graphics::Graphics() {
        this->window = std::move(std::unique_ptr<Window>(new Window("Universe Engine")));
    }

    const Window& Graphics::GetWindow() const {
        return *this->window.get();
    }

    void Graphics::Update() {
        this->BuildRenderables();

        this->window->Update();
    }

    void Graphics::BuildRenderables() {
        World& world = Engine::GetWorld();
        Resources& resources = Engine::GetResources();

        auto sceneInstances = world.AllSceneInstances();
        for (SceneInstance& sceneInstance : sceneInstances) {
            Scene& scene = resources.GetScene(sceneInstance.hScene).Value();

            for (Mesh& mesh : scene.meshes) {
                if (!mesh.renderable.has_value()) {
                    mesh.renderable = MeshRenderable(mesh);
                }
            }
        }
    }
}  // namespace UniverseEngine