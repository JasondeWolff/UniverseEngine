#include "Graphics.h"

#include "Engine.h"
#include "Format.h"

struct VPUniformBuffer {
    glm::mat4 view;
    glm::mat4 proj;
};

struct PushConstant {
    glm::mat4 model;
    glm::mat4 invTransModel;
};

struct ShaderPointLight {
    glm::vec4 position;
    glm::vec4 color;
    float intensity;

    float PADDING[3];
};

struct LightingUniformBuffer {
    ShaderPointLight pointLights[16];

    glm::vec4 eyePosition;

    int pointLightCount;

    float PADDING[3];
};

namespace UniverseEngine {
    Graphics::Graphics() {
        bool enableDebug = true;

        this->window = std::move(std::unique_ptr<Window>(new Window("Universe Engine")));
        this->instance = std::make_shared<GraphicsInstance>(*this->window, enableDebug);
        this->physicalDevice = std::make_unique<PhysicalDevice>(*this->instance);
        this->device =
            std::make_shared<LogicalDevice>(this->instance, *this->physicalDevice, enableDebug);
        this->cmdQueue =
            std::make_unique<CmdQueue>(this->device, *this->physicalDevice, QueueType::GRAPHICS);
        this->presentQueue =
            std::make_unique<CmdQueue>(this->device, *this->physicalDevice, QueueType::PRESENT);
        this->descriptorPool = std::make_shared<DescriptorPool>(this->device);

        this->sampler = std::make_shared<Sampler>("Sampler", this->device, *this->physicalDevice);

        this->BuildDescriptors();
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

        // Resize & get camera matrices
        Rect2D swapchainExtent = this->swapchain->Extent();
        uint32_t width = GetWindow().Width();
        uint32_t height = GetWindow().Height();
        camera.SetAspect(static_cast<float>(swapchainExtent.extent.x) /
                         static_cast<float>(swapchainExtent.extent.y));
        const glm::mat4& viewMatrix = glm::inverse(camera.transform.GetMatrix());
        const glm::mat4& projectionMatrix = camera.GetMatrix();

        // Wait for next image
        auto fence = this->swapchain->NextImage();
        size_t currentFrame = static_cast<size_t>(this->swapchain->GetCurrentFrameIdx());

        // Update camera matrices
        VPUniformBuffer uniformBuffer{viewMatrix, projectionMatrix};
        void* uniformBufferData = this->vpUniformBuffers[currentFrame]->Map();
        memcpy(uniformBufferData, &uniformBuffer, sizeof(VPUniformBuffer));
        this->vpUniformBuffers[currentFrame]->Unmap();

        std::shared_ptr<CmdList> cmdList = this->cmdQueue->GetCmdList();

        this->UpdateMaterials(currentFrame, *cmdList);

        cmdList->BeginRenderPass(this->renderPass, this->swapchain->GetCurrentFramebuffer(),
                                 glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

        cmdList->BindGraphicsPipeline(this->unlitPipeline);

        cmdList->SetScissor(swapchainExtent);
        cmdList->SetViewport(swapchainExtent);

        // Update lights
        LightingUniformBuffer lightingUniformBufferData{};
        lightingUniformBufferData.eyePosition = glm::vec4(camera.transform.GetTranslation(), 1.0f);
        auto& sceneInstances = world.GetAllSceneInstances();
        for (auto& sceneInstance : sceneInstances) {
            auto& scene = sceneInstance->hScene;

            auto sceneHierarchy = scene->TransformedHierarchy(sceneInstance->transform);
            for (auto& sceneNode : sceneHierarchy) {
                if (!sceneNode.pointLightIdx.has_value())
                    continue;
                PointLight& pointLight = scene->pointLights[sceneNode.pointLightIdx.value()];

                ShaderPointLight shaderPointLight;
                shaderPointLight.position = glm::vec4(sceneNode.transform.GetTranslation(), 1.0f);
                shaderPointLight.color = glm::vec4(pointLight.color, 1.0f);
                shaderPointLight.intensity = pointLight.intensity;

                lightingUniformBufferData.pointLights[lightingUniformBufferData.pointLightCount++] =
                    shaderPointLight;
            }
        }
        uniformBufferData = this->lightingUniformBuffers[currentFrame]->Map();
        memcpy(uniformBufferData, &lightingUniformBufferData, sizeof(LightingUniformBuffer));
        this->lightingUniformBuffers[currentFrame]->Unmap();

        for (auto& sceneInstance : sceneInstances) {
            auto& scene = sceneInstance->hScene;

            auto meshHierarchy = scene->TransformedHierarchy(sceneInstance->transform);
            for (auto& meshInstance : meshHierarchy) {
                if (!meshInstance.meshIdx.has_value())
                    continue;

                Mesh& mesh = scene->meshes[meshInstance.meshIdx.value()];
                PushConstant pushConstant;
                pushConstant.model = meshInstance.transform.GetMatrix();
                pushConstant.invTransModel = glm::transpose(glm::inverse(pushConstant.model));

                cmdList->BindDescriptorSet(this->vpDescriptorSets[currentFrame], 0);
                cmdList->BindDescriptorSet(this->lightingDescriptorSets[currentFrame], 2);

                Material& material = scene->materials[mesh.materialIdx];
                material.renderable->Bind(*cmdList, currentFrame, 1);

                cmdList->PushConstant("pc", pushConstant, GraphicsStageFlagBits::VERTEX_STAGE);

                mesh.renderable->Draw(*cmdList);
            }
        }

        cmdList->EndRenderPass();

        std::vector<Semaphore*> waitSemaphores{&this->swapchain->GetImageAvailableSemaphore()};
        std::vector<Semaphore*> signalSemaphores{&this->swapchain->GetRenderFinishedSemaphore()};
        this->cmdQueue->SubmitCmdList(cmdList, fence, waitSemaphores, signalSemaphores);

        this->swapchain->Present(*this->presentQueue, *fence, signalSemaphores);
    }

    void Graphics::BuildDescriptors() {
        this->vpDescriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            this->device,
            std::vector<DescriptorLayoutBinding>{DescriptorLayoutBinding(
                "ubo", 0, DescriptorType::UNIFORM_BUFFER, GraphicsStageFlagBits::VERTEX_STAGE)});
        this->lightingDescriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            this->device, std::vector<DescriptorLayoutBinding>{
                              DescriptorLayoutBinding("lighting", 8, DescriptorType::UNIFORM_BUFFER,
                                                      GraphicsStageFlagBits::FRAGMENT_STAGE)});

        for (size_t i = 0; i < this->vpUniformBuffers.size(); i++) {
            this->vpUniformBuffers[i] =
                std::make_shared<Buffer>(Format("vpUniformBuffer_%i", i), this->device,
                                         *this->physicalDevice, BufferUsageBits::UNIFORM_BUFFER,
                                         sizeof(VPUniformBuffer), BufferLocation::CPU_TO_GPU);

            this->vpDescriptorSets[i] = std::make_shared<DescriptorSet>(
                this->device, this->descriptorPool, this->vpDescriptorSetLayout);

            this->vpDescriptorSets[i]->SetBuffer(0, DescriptorType::UNIFORM_BUFFER,
                                                 this->vpUniformBuffers[i]);
        }
        for (size_t i = 0; i < this->lightingUniformBuffers.size(); i++) {
            this->lightingUniformBuffers[i] =
                std::make_shared<Buffer>(Format("lightingUniformBuffer_%i", i), this->device,
                                         *this->physicalDevice, BufferUsageBits::UNIFORM_BUFFER,
                                         sizeof(LightingUniformBuffer), BufferLocation::CPU_TO_GPU);

            this->lightingDescriptorSets[i] = std::make_shared<DescriptorSet>(
                this->device, this->descriptorPool, this->lightingDescriptorSetLayout);

            this->lightingDescriptorSets[i]->SetBuffer(8, DescriptorType::UNIFORM_BUFFER,
                                                       this->lightingUniformBuffers[i]);
        }
    }

    void Graphics::BuildSwapchain() {
        this->swapchain.reset();
        this->swapchain = std::make_unique<Swapchain>(*this->window, *this->instance, this->device,
                                                      *this->physicalDevice);

        uint32_t width = this->swapchain->Extent().extent.x;
        uint32_t height = this->swapchain->Extent().extent.y;
        this->depthImage = std::make_shared<Image>(
            "Depth Image", this->device, *this->physicalDevice, width, height, 1,
            ImageUsageBits::DEPTH_STENCIL_ATTACHMENT, GraphicsFormat::D32_SFLOAT);

        this->renderPass = std::make_shared<RenderPass>(
            this->device, std::vector<GraphicsFormat>{this->swapchain->Format()},
            std::make_optional<GraphicsFormat>(this->depthImage->Format()));
        this->swapchain->RebuildFramebuffers(this->renderPass, this->depthImage);
    }

    void Graphics::BuildPipelines() {
        auto& resources = Engine::GetResources();
        std::shared_ptr<Shader> hShaderUnlitVS = resources.LoadShader("Assets/Shaders/unlit.vert");
        std::shared_ptr<Shader> hShaderUnlitFS = resources.LoadShader("Assets/Shaders/unlit.frag");
        this->BuildRenderables();

        std::vector<ShaderRenderable*> unlitShaders = {hShaderUnlitVS->renderable.get(),
                                                       hShaderUnlitFS->renderable.get()};
        this->unlitPipeline = std::make_shared<GraphicsPipeline>(
            this->device, unlitShaders, this->renderPass,
            std::vector<std::shared_ptr<DescriptorSetLayout>>{
                vpDescriptorSetLayout, MaterialRenderable::DescriptorLayout(this->device),
                lightingDescriptorSetLayout},
            std::vector<PushConstantRange>{PushConstantRange("pc", sizeof(PushConstant),
                                                             GraphicsStageFlagBits::VERTEX_STAGE)});
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
                    if (!mesh.HasTangents())
                        mesh.GenerateTangents();

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

    void Graphics::UpdateMaterials(size_t currentFrame, CmdList& cmdList) {
        Resources& resources = Engine::GetResources();

        auto& scenes = resources.GetAllScenes();
        for (auto& scene : scenes) {
            for (Material& material : scene->materials) {
                if (!material.renderable) {
                    material.renderable =
                        std::move(std::unique_ptr<MaterialRenderable>(new MaterialRenderable(
                            this->device, *this->physicalDevice, this->descriptorPool, material)));
                }

                material.renderable->Update(currentFrame, this->sampler, cmdList);
            }
        }
    }
}  // namespace UniverseEngine