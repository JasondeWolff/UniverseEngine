#include "Graphics.h"

#include "Engine.h"

namespace UniverseEngine {
    Graphics::Graphics() {
        this->window = std::move(std::unique_ptr<Window>(new Window("Universe Engine")));

        auto& resources = Engine::GetResources();
        this->hShaderUnlit = resources.LoadShader("Assets/Shaders/unlit.vs");
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

        auto hShaders = resources.GetNewShaders();
        for (auto hShader : hShaders) {
            auto optionalShader = resources.GetShader(hShader);
            if (!optionalShader)
                continue;

            Shader& shader = optionalShader.Value();

            if (shader.renderable.has_value()) {
                shader.renderable = ShaderRenderable(shader);
                shader.ClearCPUData();
            }
        }

        auto& hSceneInstances = world.newInstances;
        for (auto hSceneInstance : hSceneInstances) {
            auto optiontalSceneInstance = world.GetSceneInstance(hSceneInstance);
            if (!optiontalSceneInstance)
                continue;

            SceneInstance& sceneInstance = optiontalSceneInstance.Value();
            Scene& scene = resources.GetScene(sceneInstance.hScene).Value();

            for (Mesh& mesh : scene.meshes) {
                if (!mesh.renderable.has_value()) {
                    mesh.renderable = MeshRenderable(mesh);
                    mesh.ClearCPUData();
                }
            }
        }
        world.newInstances.clear();
    }
}  // namespace UniverseEngine