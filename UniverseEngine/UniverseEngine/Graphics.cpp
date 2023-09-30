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

struct ShaderDirectionalLight {
    glm::vec4 direction;
    glm::vec4 color;
    float intensity;

    float PADDING[3];
};

struct LightingUniformBuffer {
    ShaderPointLight pointLights[16];
    ShaderDirectionalLight directionalLight;

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
        this->skyboxSampler =
            std::make_shared<Sampler>("Skybox Sampler", this->device, *this->physicalDevice);

        this->skyboxCube =
            Engine::GetResources().LoadScene("Assets/Models/SkyboxCube/SkyboxCube.gltf");

        this->BuildDescriptors();
        this->BuildSwapchain();
        this->BuildPipelines();

        this->imguiRenderer = std::make_unique<ImGuiRenderer>(
            this->device, *this->physicalDevice, this->descriptorPool, this->renderPass, *this);
    }

    Graphics::~Graphics() {
        this->device->WaitIdle();
    }

    const Window& Graphics::GetWindow() const {
        return *this->window.get();
    }

    void Graphics::SetSkybox(std::array<std::shared_ptr<Texture>, 6> textures) {
        this->skyboxTextures = textures;
    }

    void Graphics::Update() {
        this->RebuildShaders();
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

        ShaderDirectionalLight shaderDirectionalLight{};
        shaderDirectionalLight.direction = glm::vec4(world.sun.direction, 1.0f);
        shaderDirectionalLight.color = glm::vec4(world.sun.lightSource.color, 1.0f);
        shaderDirectionalLight.intensity = world.sun.lightSource.intensity;
        lightingUniformBufferData.directionalLight = shaderDirectionalLight;

        uniformBufferData = this->lightingUniformBuffers[currentFrame]->Map();
        memcpy(uniformBufferData, &lightingUniformBufferData, sizeof(LightingUniformBuffer));
        this->lightingUniformBuffers[currentFrame]->Unmap();

        cmdList->BindGraphicsPipeline(this->skyboxPipeline);

        cmdList->BindDescriptorSet(this->vpDescriptorSets[currentFrame], 0);
        this->skyboxDescriptorSets[currentFrame]->SetImage(
            1, DescriptorType::COMBINED_IMAGE_SAMPLER, this->skyboxImage, this->skyboxSampler);
        cmdList->BindDescriptorSet(this->skyboxDescriptorSets[currentFrame], 1);
        this->skyboxCube->meshes[0].renderable->Draw(*cmdList);

        cmdList->BindGraphicsPipeline(this->pbrPipeline);

        cmdList->BindDescriptorSet(this->vpDescriptorSets[currentFrame], 0);
        cmdList->BindDescriptorSet(this->lightingDescriptorSets[currentFrame], 2);

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

                Material& material = scene->materials[mesh.materialIdx];
                material.renderable->Bind(*cmdList, currentFrame, 1);

                cmdList->PushConstant("pc", pushConstant, GraphicsStageFlagBits::VERTEX_STAGE);

                mesh.renderable->Draw(*cmdList);
            }
        }

        this->imguiRenderer->Render(*cmdList, currentFrame);

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
        this->skyboxDescriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            this->device, std::vector<DescriptorLayoutBinding>{DescriptorLayoutBinding(
                              "skyboxMap", 1, DescriptorType::COMBINED_IMAGE_SAMPLER,
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
        for (size_t i = 0; i < this->skyboxDescriptorSets.size(); i++) {
            this->skyboxDescriptorSets[i] = std::make_shared<DescriptorSet>(
                this->device, this->descriptorPool, this->skyboxDescriptorSetLayout);
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
        std::shared_ptr<Shader> shaderPbrVS = resources.LoadShader("Assets/Shaders/unlit.vert");
        std::shared_ptr<Shader> shaderPbrFS = resources.LoadShader("Assets/Shaders/unlit.frag");
        std::shared_ptr<Shader> shaderSkyboxVS = resources.LoadShader("Assets/Shaders/skybox.vert");
        std::shared_ptr<Shader> shaderSkyboxFS = resources.LoadShader("Assets/Shaders/skybox.frag");
        this->RebuildShaders();

        GraphicsPipelineInfo pbrInfo{};
        std::vector<const ShaderRenderable*> pbrShaders = {shaderPbrVS->renderable.get(),
                                                     shaderPbrFS->renderable.get()};
        this->pbrPipeline = std::make_shared<GraphicsPipeline>(
            this->device, pbrShaders, this->renderPass,
            std::vector<std::shared_ptr<DescriptorSetLayout>>{
                vpDescriptorSetLayout, MaterialRenderable::DescriptorLayout(this->device),
                lightingDescriptorSetLayout},
            std::vector<PushConstantRange>{
                PushConstantRange("pc", sizeof(PushConstant), GraphicsStageFlagBits::VERTEX_STAGE)},
            pbrInfo);

        GraphicsPipelineInfo skyboxInfo{};
        skyboxInfo.ignoreDepth = true;
        std::vector<const ShaderRenderable*> skyboxShaders = {shaderSkyboxVS->renderable.get(),
                                                        shaderSkyboxFS->renderable.get()};
        this->skyboxPipeline = std::make_shared<GraphicsPipeline>(
            this->device, skyboxShaders, this->renderPass,
            std::vector<std::shared_ptr<DescriptorSetLayout>>{vpDescriptorSetLayout,
                                                              skyboxDescriptorSetLayout},
            std::vector<PushConstantRange>{}, skyboxInfo);
    }

    void Graphics::RebuildShaders() const {
        Resources& resources = Engine::GetResources();
        auto& shaders = resources.GetNewShaders();
        for (auto& shader : shaders) {
            if (!shader->renderable) {
                shader->renderable = std::move(
                    std::unique_ptr<ShaderRenderable>(new ShaderRenderable(this->device, *shader)));
                shader->ClearCPUData();
            }
        }
    }

    void Graphics::BuildRenderables() {
        World& world = Engine::GetWorld();
        Resources& resources = Engine::GetResources();
        std::shared_ptr<CmdList> uploadCmdList = this->cmdQueue->GetCmdList();

        auto& textures = resources.GetNewTextures();
        for (auto& texture : textures) {
            if (!texture->renderable) {
                texture->renderable =
                    std::move(std::unique_ptr<TextureRenderable>(new TextureRenderable(
                        this->device, *this->physicalDevice, *uploadCmdList, *texture)));
                texture->ClearCPUData();
            }
        }

        if (this->skyboxTextures[0]) {
            this->skyboxImage.reset();
            this->skyboxImage = std::make_shared<Image>(
                "Skybox", this->device, *this->physicalDevice, this->skyboxTextures[0]->width,
                this->skyboxTextures[0]->height, this->skyboxTextures[0]->mips,
                ImageUsageBits::TRANSFER_DST_IMAGE | ImageUsageBits::SAMPLED_IMAGE,
                GraphicsFormat::R8G8B8A8_SRGB, 6);

            std::array<std::shared_ptr<Image>, 6> skyboxImages{
                this->skyboxTextures[0]->renderable->GetImage(),
                this->skyboxTextures[1]->renderable->GetImage(),
                this->skyboxTextures[2]->renderable->GetImage(),
                this->skyboxTextures[3]->renderable->GetImage(),
                this->skyboxTextures[4]->renderable->GetImage(),
                this->skyboxTextures[5]->renderable->GetImage()};
            uploadCmdList->CopyImagesIntoCubemap(skyboxImages, skyboxImage);

            for (auto& skyboxTexture : this->skyboxTextures) {
                skyboxTexture.reset();
            }
        }

        auto& scenes = resources.GetNewScenes();
        for (auto& scene : scenes) {
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