#include "Graphics.h"

#include "Engine.h"

namespace UniverseEngine {
    Graphics::Graphics() {
        this->window = std::move(std::unique_ptr<Window>(new Window("Universe Engine")));

        auto& resources = Engine::GetResources();

        Handle<Shader> hShaderUnlitVS = resources.LoadShader("Assets/Shaders/unlit.vs");
        Handle<Shader> hShaderUnlitFS = resources.LoadShader("Assets/Shaders/unlit.fs");
        this->BuildRenderables();

        std::vector<ShaderRenderable*> unlitShaders = {
            resources.GetShader(hShaderUnlitVS).Value().renderable.get(),
            resources.GetShader(hShaderUnlitFS).Value().renderable.get()};
        this->unlitPipeline = std::make_unique<GraphicsPipeline>(unlitShaders);
        resources.DeleteShader(hShaderUnlitVS);
        resources.DeleteShader(hShaderUnlitFS);
    }

    const Window& Graphics::GetWindow() const {
        return *this->window.get();
    }

    void Graphics::Update() {
        this->BuildRenderables();

        World& world = Engine::GetWorld();
        Resources& resources = Engine::GetResources();

        uint32_t width = GetWindow().Width();
        uint32_t height = GetWindow().Height();

        CmdList cmdList{};

        cmdList.SetScissor(Rect2D(width, height));
        cmdList.SetViewport(Rect2D(width, height));

        cmdList.Clear(glm::vec4(1.0, 0.0, 1.0, 1.0));

        auto& hSceneInstances = world.newInstances;
        for (auto hSceneInstance : hSceneInstances) {
            auto optiontalSceneInstance = world.GetSceneInstance(hSceneInstance);
            if (!optiontalSceneInstance)
                continue;

            SceneInstance& sceneInstance = optiontalSceneInstance.Value();
            Scene& scene = resources.GetScene(sceneInstance.hScene).Value();

            for (Mesh& mesh : scene.meshes) {
                mesh.renderable->Draw(cmdList);
            }
        }

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

            if (!shader.renderable) {
                shader.renderable =
                    std::move(std::unique_ptr<ShaderRenderable>(new ShaderRenderable(shader)));
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
                if (!mesh.renderable) {
                    mesh.renderable =
                        std::move(std::unique_ptr<MeshRenderable>(new MeshRenderable(mesh)));
                    mesh.ClearCPUData();
                }
            }
        }
        world.newInstances.clear();
    }
}  // namespace UniverseEngine