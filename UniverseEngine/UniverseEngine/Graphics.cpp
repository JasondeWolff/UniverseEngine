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
        this->computeQueue =
            std::make_unique<CmdQueue>(this->device, *this->physicalDevice, QueueType::COMPUTE);
        this->presentQueue =
            std::make_unique<CmdQueue>(this->device, *this->physicalDevice, QueueType::PRESENT);
        this->descriptorPool = std::make_shared<DescriptorPool>(this->device);

        this->sampler = std::make_shared<Sampler>("Sampler", this->device, *this->physicalDevice);
        this->skyboxSampler =
            std::make_shared<Sampler>("Skybox Sampler", this->device, *this->physicalDevice);

        this->skyboxCube =
            Engine::GetResources().LoadScene("Assets/Models/SkyboxCube/SkyboxCube.gltf");

        this->BuildSwapchain();
        this->BuildDescriptors();
        this->BuildPipelines();

        this->cloudRenderer = std::make_unique<CloudRenderer>(this->device, *this->physicalDevice,
                                                              this->descriptorPool, *this);
        this->imguiRenderer = std::make_unique<ImGuiRenderer>(
            this->device, *this->physicalDevice, this->descriptorPool,
            this->swapchain->GetRenderPass(), *this);
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

    void Graphics::SetPolygonMode(GraphicsPolygonMode polygonMode) {
        if (this->polygonMode != polygonMode) {
            this->polygonMode = polygonMode;

            this->device->WaitIdle();
            this->BuildPipelines();
        }
    }

    void Graphics::UseLODs(bool useLODs) {
        this->useLODs = useLODs;
    }

    CloudConfig Graphics::GetCloudConfig() const {
        return this->cloudRenderer->config;
    }

    void Graphics::SetCloudConfig(const CloudConfig& config) const {
        this->cloudRenderer->config = config;
    }

    void Graphics::Update() {
        this->RebuildShaders();
        this->BuildRenderables();

        this->cmdQueue->ProcessCmdLists();
        this->computeQueue->ProcessCmdLists();
        this->presentQueue->ProcessCmdLists();

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

        std::shared_ptr<CmdList> pbrCmdList = this->cmdQueue->GetCmdList();

        this->UpdateMaterials(currentFrame, *pbrCmdList);

        pbrCmdList->BeginRenderPass(this->renderPass, *this->framebuffer,
                                    glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

        pbrCmdList->SetScissor(swapchainExtent);
        pbrCmdList->SetViewport(swapchainExtent);

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

        if (this->skyboxImage) {
            pbrCmdList->BindGraphicsPipeline(this->skyboxPipeline);

            pbrCmdList->BindDescriptorSet(this->vpDescriptorSets[currentFrame], 0);
            this->skyboxDescriptorSets[currentFrame]->SetImage(
                1, DescriptorType::COMBINED_IMAGE_SAMPLER, this->skyboxImage, this->skyboxSampler);
            pbrCmdList->BindDescriptorSet(this->skyboxDescriptorSets[currentFrame], 1);
            this->skyboxCube->meshes[0].lods[0].renderable->Draw(*pbrCmdList);
        }

        pbrCmdList->BindGraphicsPipeline(this->pbrPipeline);

        pbrCmdList->BindDescriptorSet(this->vpDescriptorSets[currentFrame], 0);
        pbrCmdList->BindDescriptorSet(this->lightingDescriptorSets[currentFrame], 2);

        for (auto& sceneInstance : sceneInstances) {
            auto& scene = sceneInstance->hScene;

            auto meshHierarchy = scene->TransformedHierarchy(sceneInstance->transform);
            for (auto& meshInstance : meshHierarchy) {
                if (!meshInstance.meshIdx.has_value())
                    continue;

                LODMesh& lodMesh = scene->meshes[meshInstance.meshIdx.value()];
                size_t lodIdx = 0;
                if (this->useLODs) {
                    auto lodIdxOpt = lodMesh.BestLOD(camera.transform.GetTranslation(),
                                                     meshInstance.transform.GetTranslation());
                    if (!lodIdxOpt.has_value())
                        continue;
                    lodIdx = lodIdxOpt.value();
                }

                Mesh& mesh = lodMesh.lods[lodIdx];
                PushConstant pushConstant;
                pushConstant.model = meshInstance.transform.GetMatrix();
                pushConstant.invTransModel = glm::transpose(glm::inverse(pushConstant.model));

                Material& material = scene->materials[mesh.materialIdx];
                material.renderable->Bind(*pbrCmdList, currentFrame, 1);

                pbrCmdList->PushConstant("pc", pushConstant, GraphicsStageFlagBits::VERTEX_STAGE);

                mesh.renderable->Draw(*pbrCmdList);
            }
        }

        pbrCmdList->EndRenderPass();

        std::vector<Semaphore*> waitSemaphores{&this->swapchain->GetImageAvailableSemaphore()};
        std::vector<Semaphore*> signalSemaphores{
            &this->cloudRenderer->CurrentSemaphore(currentFrame)};
        this->cmdQueue->SubmitCmdList(pbrCmdList, nullptr, waitSemaphores, signalSemaphores);

        std::shared_ptr<CmdList> computeCmdList = this->computeQueue->GetCmdList();
        this->cloudRenderer->Render(*computeCmdList, this->colorImage, currentFrame);
        std::vector<Semaphore*> cloudWaitSemaphores{
            &this->cloudRenderer->CurrentSemaphore(currentFrame)};
        std::vector<Semaphore*> cloudSignalSemaphores{};
        this->computeQueue->SubmitCmdList(computeCmdList, nullptr, cloudWaitSemaphores,
                                          cloudSignalSemaphores);

        std::shared_ptr<CmdList> presentCmdList = this->cmdQueue->GetCmdList();

        presentCmdList->TransitionImageLayout(this->colorImage, ImageLayout::PRESENT_SRC,
                                              ImageLayout::SHADER_READ_ONLY_OPTIMAL);

        presentCmdList->BeginRenderPass(this->swapchain->GetRenderPass(),
                                        this->swapchain->GetCurrentFramebuffer(),
                                        glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        presentCmdList->SetScissor(swapchainExtent);
        presentCmdList->SetViewport(swapchainExtent);
        presentCmdList->BindGraphicsPipeline(this->presentPipeline);
        presentCmdList->BindDescriptorSet(this->presentDescriptorSets[currentFrame], 0);
        presentCmdList->Draw(3);
        this->imguiRenderer->Render(*presentCmdList, currentFrame);
        presentCmdList->EndRenderPass();

        presentCmdList->TransitionImageLayout(
            this->colorImage, ImageLayout::SHADER_READ_ONLY_OPTIMAL, ImageLayout::PRESENT_SRC);

        std::vector<Semaphore*> presentWaitSemaphores{};
        std::vector<Semaphore*> presentSignalSemaphores{
            &this->swapchain->GetRenderFinishedSemaphore()};
        this->cmdQueue->SubmitCmdList(presentCmdList, fence, presentWaitSemaphores,
                                      presentSignalSemaphores);

        this->swapchain->Present(*this->presentQueue, *fence, presentSignalSemaphores);
    }

    void Graphics::BuildDescriptors() {
        this->presentDescriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            this->device, std::vector<DescriptorLayoutBinding>{DescriptorLayoutBinding(
                              "presentImage", 0, DescriptorType::COMBINED_IMAGE_SAMPLER,
                              GraphicsStageFlagBits::FRAGMENT_STAGE)});
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

        for (size_t i = 0; i < this->presentDescriptorSets.size(); i++) {
            this->presentDescriptorSets[i] = std::make_shared<DescriptorSet>(
                this->device, this->descriptorPool, this->presentDescriptorSetLayout);
            this->presentDescriptorSets[i]->SetImage(0, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                     this->colorImage, this->sampler);
        }
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

        uint32_t width = static_cast<uint32_t>(this->swapchain->Extent().extent.x);
        uint32_t height = static_cast<uint32_t>(this->swapchain->Extent().extent.y);
        this->colorImage = std::make_shared<Image>(
            "Color Image", this->device, *this->physicalDevice, width, height, 1,
            ImageUsageBits::COLOR_ATTACHMENT | ImageUsageBits::SAMPLED_IMAGE |
                ImageUsageBits::STORAGE_IMAGE,
            GraphicsFormat::R8G8B8A8_UNORM);
        this->depthImage = std::make_shared<Image>(
            "Depth Image", this->device, *this->physicalDevice, width, height, 1,
            ImageUsageBits::DEPTH_STENCIL_ATTACHMENT, GraphicsFormat::D32_SFLOAT);

        this->renderPass = std::make_shared<RenderPass>(
            this->device, std::vector<GraphicsFormat>{this->colorImage->Format()},
            std::make_optional<GraphicsFormat>(this->depthImage->Format()));
        std::vector<std::shared_ptr<Image>> images{this->colorImage, this->depthImage};
        this->framebuffer = std::make_unique<Framebuffer>(this->device, images, this->renderPass);
    }

    void Graphics::BuildPipelines() {
        auto& resources = Engine::GetResources();
        std::shared_ptr<Shader> shaderPresentVS =
            resources.LoadShader("Assets/Shaders/present.vert");
        std::shared_ptr<Shader> shaderPresentFS =
            resources.LoadShader("Assets/Shaders/present.frag");
        std::shared_ptr<Shader> shaderPbrVS = resources.LoadShader("Assets/Shaders/unlit.vert");
        std::shared_ptr<Shader> shaderPbrFS = resources.LoadShader("Assets/Shaders/unlit.frag");
        std::shared_ptr<Shader> shaderSkyboxVS = resources.LoadShader("Assets/Shaders/skybox.vert");
        std::shared_ptr<Shader> shaderSkyboxFS = resources.LoadShader("Assets/Shaders/skybox.frag");
        this->RebuildShaders();

        GraphicsPipelineInfo presentInfo{};
        presentInfo.ignoreDepth = true;
        presentInfo.inputVertices = false;
        std::vector<const ShaderRenderable*> presentShaders = {shaderPresentVS->renderable.get(),
                                                               shaderPresentFS->renderable.get()};
        this->presentPipeline = std::make_shared<GraphicsPipeline>(
            this->device, presentShaders, this->swapchain->GetRenderPass(),
            std::vector<std::shared_ptr<DescriptorSetLayout>>{presentDescriptorSetLayout},
            std::vector<PushConstantRange>{}, presentInfo);

        GraphicsPipelineInfo pbrInfo{};
        pbrInfo.polygonMode = this->polygonMode;
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
            for (LODMesh& lodMesh : scene->meshes) {
                for (size_t i = 0; i < lodMesh.lods.size(); i++) {
                    Mesh& mesh = lodMesh.lods[i];

                    if (!mesh.renderable) {
                        if (!mesh.HasTangents())
                            mesh.GenerateTangents();

                        mesh.renderable =
                            std::move(std::unique_ptr<MeshRenderable>(new MeshRenderable(
                                this->device, *this->physicalDevice, *uploadCmdList, mesh)));
                        mesh.ClearCPUData();
                    }
                }
            }
        }
        world.newInstances.clear();

        // TODO: Use signal semaphore to not block entire cpu
        auto fence = std::make_shared<Fence>(this->device);
        this->cmdQueue->SubmitCmdList(uploadCmdList, fence, {}, {});
        fence->Wait();
    }

    void Graphics::UpdateMaterials(size_t currentFrame, CmdList& pbrCmdList) {
        Resources& resources = Engine::GetResources();

        auto& scenes = resources.GetAllScenes();
        for (auto& scene : scenes) {
            for (Material& material : scene->materials) {
                if (!material.renderable) {
                    material.renderable =
                        std::move(std::unique_ptr<MaterialRenderable>(new MaterialRenderable(
                            this->device, *this->physicalDevice, this->descriptorPool, material)));
                }

                material.renderable->Update(currentFrame, this->sampler, pbrCmdList);
            }
        }
    }
}  // namespace UniverseEngine