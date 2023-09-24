#include "Graphics.h"

#include "Engine.h"
#include "Format.h"

struct MVPUniformBuffer {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

namespace UniverseEngine {
    Graphics::Graphics() {
        bool enableDebug = true;

        this->window = std::move(std::unique_ptr<Window>(new Window("Universe Engine")));
        this->instance = std::make_shared<GraphicsInstance>(*this->window, enableDebug);
        this->physicalDevice = std::make_unique<PhysicalDevice>(*this->instance);
        this->device =
            std::make_shared<LogicalDevice>(this->instance, *this->physicalDevice, enableDebug);
        this->cmdQueue = std::make_unique<CmdQueue>(this->device, *this->physicalDevice);
        this->descriptorPool = std::make_shared<DescriptorPool>(this->device);

        this->descriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            this->device,
            std::vector<DescriptorLayoutBinding>{
                DescriptorLayoutBinding("ubo", 0, DescriptorType::UNIFORM_BUFFER,
                                        DescriptorStageFlagBits::VERTEX),
                DescriptorLayoutBinding("baseColorMap", 1, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                        DescriptorStageFlagBits::FRAGMENT)});

        for (size_t i = 0; i < this->uniformBuffers.size(); i++) {
            this->uniformBuffers[i] =
                std::make_shared<Buffer>(Format("MVPUniformBuffer_%i", i), this->device,
                                         *this->physicalDevice, BufferUsageBits::UNIFORM_BUFFER,
                                         sizeof(MVPUniformBuffer), BufferLocation::GPU_ONLY);

            this->descriptorSets[i] = std::make_shared<DescriptorSet>(
                this->device, this->descriptorPool, this->descriptorSetLayout);

            this->descriptorSets[i]->SetBuffer(0, DescriptorType::UNIFORM_BUFFER,
                                               *this->uniformBuffers[i]);
        }

        this->sampler = std::make_shared<Sampler>("Sampler", this->device, *this->physicalDevice);

        this->BuildSwapchain();
        this->BuildPipelines();
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

        if (!this->window->IsMinimized()) {
            if (this->window->WasResized()) {
                this->device->WaitIdle();
                this->BuildSwapchain();
                this->BuildPipelines();
            }

            this->Render();
        }

        this->window->Update();
    }

    void Graphics::Render() {
        World& world = Engine::GetWorld();
        Camera& camera = world.camera;
        Resources& resources = Engine::GetResources();

        Rect2D swapchainExtent = this->swapchain->Extent();
        uint32_t width = GetWindow().Width();
        uint32_t height = GetWindow().Height();
        camera.SetAspect(static_cast<float>(swapchainExtent.extent.x) /
                         static_cast<float>(swapchainExtent.extent.y));

        const glm::mat4& viewMatrix = glm::inverse(camera.transform.GetMatrix());
        const glm::mat4& projectionMatrix = camera.GetMatrix();

        auto fence = this->swapchain->NextImage();
        size_t currentFrame = static_cast<size_t>(this->swapchain->GetCurrentFrameIdx());

        std::shared_ptr<CmdList> cmdList = this->cmdQueue->GetCmdList();

        cmdList->BeginRenderPass(this->renderPass, this->swapchain->GetCurrentFramebuffer(),
                                 glm::vec4(0.0, 0.05, 0.07, 1.0));

        cmdList->BindGraphicsPipeline(this->unlitPipeline);

        cmdList->SetScissor(swapchainExtent);
        cmdList->SetViewport(swapchainExtent);

        auto sceneInstances = world.GetAllSceneInstances();
        for (auto sceneInstance : sceneInstances) {
            auto scene = sceneInstance->hScene;

            for (Mesh& mesh : scene->meshes) {
                MVPUniformBuffer uniformBuffer{glm::mat4(1.0), viewMatrix, projectionMatrix};

                void* uniformBufferData = this->uniformBuffers[currentFrame]->Map();
                memcpy(uniformBufferData, &uniformBuffer, sizeof(MVPUniformBuffer));
                this->uniformBuffers[currentFrame]->Unmap();

                Material& material = scene->materials[mesh.materialIdx];
                auto& baseColorImage = material.baseColorMap->renderable->image;
                this->descriptorSets[currentFrame]->SetImage(
                    1, DescriptorType::COMBINED_IMAGE_SAMPLER, baseColorImage, this->sampler);

                cmdList->BindDescriptorSet(this->descriptorSets[currentFrame]);

                // cmdList->PushConstant("UniformBufferObject", uniformBuffer);
                mesh.renderable->Draw(*cmdList);
            }
        }

        cmdList->EndRenderPass();

        std::vector<Semaphore*> waitSemaphores{&this->swapchain->GetImageAvailableSemaphore()};
        std::vector<Semaphore*> signalSemaphores{&this->swapchain->GetRenderFinishedSemaphore()};
        this->cmdQueue->SubmitCmdList(cmdList, fence, waitSemaphores, signalSemaphores);

        this->swapchain->Present(*this->cmdQueue, *fence, signalSemaphores);
    }

    void Graphics::BuildSwapchain() {
        this->swapchain.reset();
        this->swapchain = std::make_unique<Swapchain>(*this->window, *this->instance, this->device,
                                                      *this->physicalDevice);
        this->renderPass = std::make_shared<RenderPass>(this->device, this->swapchain->Format());
        this->swapchain->RebuildFramebuffers(this->renderPass);
    }

    void Graphics::BuildPipelines() {
        auto& resources = Engine::GetResources();
        std::shared_ptr<Shader> hShaderUnlitVS = resources.LoadShader("Assets/Shaders/unlit.vert");
        std::shared_ptr<Shader> hShaderUnlitFS = resources.LoadShader("Assets/Shaders/unlit.frag");
        this->BuildRenderables();

        std::vector<ShaderRenderable*> unlitShaders = {hShaderUnlitVS->renderable.get(),
                                                       hShaderUnlitFS->renderable.get()};
        this->unlitPipeline = std::make_shared<GraphicsPipeline>(
            this->device, unlitShaders, this->renderPass, descriptorSetLayout);
    }

    void Graphics::BuildRenderables() {
        World& world = Engine::GetWorld();
        Resources& resources = Engine::GetResources();
        std::shared_ptr<CmdList> uploadCmdList = this->cmdQueue->GetCmdList();

        auto shaders = resources.GetNewShaders();
        for (auto shader : shaders) {
            if (!shader->renderable) {
                shader->renderable = std::move(
                    std::unique_ptr<ShaderRenderable>(new ShaderRenderable(this->device, *shader)));
                shader->ClearCPUData();
            }
        }

        auto textures = resources.GetNewTextures();
        for (auto texture : textures) {
            if (!texture->renderable) {
                texture->renderable =
                    std::move(std::unique_ptr<TextureRenderable>(new TextureRenderable(
                        this->device, *this->physicalDevice, *uploadCmdList, *texture)));
                texture->ClearCPUData();
            }
        }

        auto& sceneInstances = world.newInstances;
        for (auto sceneInstance : sceneInstances) {
            auto scene = sceneInstance->hScene;

            for (Mesh& mesh : scene->meshes) {
                if (!mesh.renderable) {
                    mesh.renderable = std::move(std::unique_ptr<MeshRenderable>(new MeshRenderable(
                        this->device, *this->physicalDevice, *uploadCmdList, mesh)));
                    mesh.ClearCPUData();
                }
            }
        }
        world.newInstances.clear();

        // TODO: Use signal semaphore to not block entire cpu
        auto fence = std::make_shared<Fence>(this->device);
        this->cmdQueue->SubmitCmdList(uploadCmdList, fence, {}, {});
        fence->Wait();
    }
}  // namespace UniverseEngine