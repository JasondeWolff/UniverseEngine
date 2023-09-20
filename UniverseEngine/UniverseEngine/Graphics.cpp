#include "Graphics.h"

#include "Engine.h"

struct MVPPushConstant {
    glm::mat4 mvp;
};

namespace UniverseEngine {
    Graphics::Graphics() {
        bool enableDebug = true;

        this->window = std::move(std::unique_ptr<Window>(new Window("Universe Engine")));
        this->instance = std::make_shared<GraphicsInstance>(*this->window, enableDebug);
        this->physicalDevice = std::make_unique<PhysicalDevice>(*this->instance);
        this->device =
            std::make_shared<LogicalDevice>(this->instance, *this->physicalDevice, enableDebug);
        this->swapchain = std::make_unique<Swapchain>(*this->window, *this->instance, this->device,
                                                      *this->physicalDevice);
        this->cmdQueue = std::make_unique<CmdQueue>(this->device, *this->physicalDevice);

        this->renderPass = std::make_shared<RenderPass>(this->device, this->swapchain->Format());
        this->swapchain->RebuildFramebuffers(this->renderPass);

        auto& resources = Engine::GetResources();

        Handle<Shader> hShaderUnlitVS = resources.LoadShader("Assets/Shaders/unlit.vert");
        Handle<Shader> hShaderUnlitFS = resources.LoadShader("Assets/Shaders/unlit.frag");
        this->BuildRenderables();

        std::vector<ShaderRenderable*> unlitShaders = {
            resources.GetShader(hShaderUnlitVS).Value().renderable.get(),
            resources.GetShader(hShaderUnlitFS).Value().renderable.get()};
        this->unlitPipeline =
            std::make_shared<GraphicsPipeline>(this->device, unlitShaders, this->renderPass);

        resources.DeleteShader(hShaderUnlitVS);
        resources.DeleteShader(hShaderUnlitFS);
    }

    Graphics::~Graphics() {
        this->device->WaitIdle();
    }

    const Window& Graphics::GetWindow() const {
        return *this->window.get();
    }

    void Graphics::Update() {
        this->BuildRenderables();
        this->cmdQueue->ProcessCmdLists();

        World& world = Engine::GetWorld();
        Camera& camera = world.camera;
        Resources& resources = Engine::GetResources();

        Rect2D swapchainExtent = this->swapchain->Extent();
        uint32_t width = GetWindow().Width();
        uint32_t height = GetWindow().Height();
        camera.SetAspect(static_cast<float>(swapchainExtent.extent.x) /
                         static_cast<float>(swapchainExtent.extent.y));

        const glm::mat4& viewMatrix = camera.transform.GetMatrix();
        const glm::mat4& projectionMatrix = camera.GetMatrix();
        const glm::mat4 vpMatrix = projectionMatrix * glm::inverse(viewMatrix);

        auto fence = this->swapchain->NextImage();

        std::shared_ptr<CmdList> cmdList = this->cmdQueue->GetCmdList();

        cmdList->BeginRenderPass(this->renderPass, this->swapchain->GetCurrentFramebuffer(),
                                 glm::vec4(0.0, 0.05, 0.07, 1.0));

        cmdList->BindGraphicsPipeline(this->unlitPipeline);

        cmdList->SetScissor(swapchainExtent);
        cmdList->SetViewport(swapchainExtent);

        auto sceneInstances = world.GetAllSceneInstances();
        for (auto sceneInstance : sceneInstances) {
            Scene& scene = resources.GetScene(sceneInstance.get().hScene).Value();

            for (Mesh& mesh : scene.meshes) {
                MVPPushConstant pushConstant{vpMatrix};

                cmdList->PushConstant("PushConstants", pushConstant);
                mesh.renderable->Draw(*cmdList);
            }
        }

        cmdList->Draw(3);

        cmdList->EndRenderPass();

        std::vector<Semaphore*> waitSemaphores{&this->swapchain->GetImageAvailableSemaphore()};
        std::vector<Semaphore*> signalSemaphores{&this->swapchain->GetRenderFinishedSemaphore()};
        this->cmdQueue->SubmitCmdList(cmdList, fence, waitSemaphores, signalSemaphores);

        this->swapchain->Present(*this->cmdQueue, *fence, signalSemaphores);

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
                shader.renderable = std::move(
                    std::unique_ptr<ShaderRenderable>(new ShaderRenderable(this->device, shader)));
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