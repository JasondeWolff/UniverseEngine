#include "CloudRenderer.h"

#include <FastNoise/FastNoise.h>

#include "Engine.h"
#include "Format.h"

struct UniformBuffer {
    glm::mat4 invView;
    glm::mat4 invProj;

    glm::vec4 lightDir;
    glm::vec4 lightColor;

    glm::vec4 cloudOffset;
    float cloudScale;
    float cloudDensityThreshold;
    float cloudDensityMultiplier;

    float forwardScattering;
    float backwardScattering;
    float baseBrightness;
    float phaseFactor;

    float lightAbsorbtion;
    float indirectLightAbsorbtion;
    float darknessThreshold;

    float zNear;
    float zFar;
};

struct NoiseUniformBuffer {
    float size;
};

struct SDFUniformBuffer {
    float densityThreshold;
};

namespace UniverseEngine {
    CloudRenderer::CloudRenderer(std::shared_ptr<LogicalDevice> device,
                                 const PhysicalDevice& physicalDevice,
                                 std::shared_ptr<DescriptorPool> descriptorPool,
                                 const Graphics& graphics)
        : device(device), physicalDevice(physicalDevice), config{} {
        this->descriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            device,
            std::vector<DescriptorLayoutBinding>{
                DescriptorLayoutBinding("outputImage", 0, DescriptorType::STORAGE_IMAGE,
                                        GraphicsStageFlagBits::COMPUTE_STAGE),
                DescriptorLayoutBinding("depthImage", 1, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                        GraphicsStageFlagBits::COMPUTE_STAGE),
                DescriptorLayoutBinding("noise", 2, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                        GraphicsStageFlagBits::COMPUTE_STAGE),
                DescriptorLayoutBinding("ubo", 3, DescriptorType::UNIFORM_BUFFER,
                                        GraphicsStageFlagBits::COMPUTE_STAGE)});
        this->noiseDescriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            device, std::vector<DescriptorLayoutBinding>{
                        DescriptorLayoutBinding("noise", 0, DescriptorType::STORAGE_IMAGE,
                                                GraphicsStageFlagBits::COMPUTE_STAGE),
                        DescriptorLayoutBinding("ubo", 1, DescriptorType::UNIFORM_BUFFER,
                                                GraphicsStageFlagBits::COMPUTE_STAGE)});
        this->sdfDescriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            device, std::vector<DescriptorLayoutBinding>{
                        DescriptorLayoutBinding("noise", 0, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                GraphicsStageFlagBits::COMPUTE_STAGE),
                        DescriptorLayoutBinding("sdf", 1, DescriptorType::STORAGE_IMAGE,
                                                GraphicsStageFlagBits::COMPUTE_STAGE),
                        DescriptorLayoutBinding("ubo", 2, DescriptorType::UNIFORM_BUFFER,
                                                GraphicsStageFlagBits::COMPUTE_STAGE)});

        for (size_t i = 0; i < this->descriptorSets.size(); i++) {
            this->uniformBuffers[i] = std::make_shared<Buffer>(
                Format("Cloud UniformBuffer_%i", i), device, physicalDevice,
                BufferUsageBits::UNIFORM_BUFFER, sizeof(UniformBuffer), BufferLocation::CPU_TO_GPU);
            this->descriptorSets[i] =
                std::make_shared<DescriptorSet>(device, descriptorPool, this->descriptorSetLayout);

            this->descriptorSets[i]->SetBuffer(3, DescriptorType::UNIFORM_BUFFER,
                                               this->uniformBuffers[i]);
        }

        this->noiseUniformBuffer = std::make_shared<Buffer>(
            "Cloud Noise UniformBuffer", device, physicalDevice, BufferUsageBits::UNIFORM_BUFFER,
            sizeof(NoiseUniformBuffer), BufferLocation::CPU_TO_GPU);
        this->noiseDescriptorSet =
            std::make_shared<DescriptorSet>(device, descriptorPool, this->noiseDescriptorSetLayout);
        this->noiseDescriptorSet->SetBuffer(1, DescriptorType::UNIFORM_BUFFER,
                                            this->noiseUniformBuffer);

        this->sdfUniformBuffer = std::make_shared<Buffer>(
            "Cloud SDF UniformBuffer", device, physicalDevice, BufferUsageBits::UNIFORM_BUFFER,
            sizeof(SDFUniformBuffer), BufferLocation::CPU_TO_GPU);
        this->sdfDescriptorSet =
            std::make_shared<DescriptorSet>(device, descriptorPool, this->sdfDescriptorSetLayout);
        this->sdfDescriptorSet->SetBuffer(2, DescriptorType::UNIFORM_BUFFER,
                                          this->sdfUniformBuffer);

        auto& resources = Engine::GetResources();
        std::shared_ptr<Shader> shader = resources.LoadShader("Assets/Shaders/clouds.comp");
        std::shared_ptr<Shader> noiseShader =
            resources.LoadShader("Assets/Shaders/cloudNoise.comp");
        std::shared_ptr<Shader> sdfShader = resources.LoadShader("Assets/Shaders/cloudSDF.comp");
        graphics.RebuildShaders();

        this->pipeline = std::make_shared<ComputePipeline>(
            device, shader->Renderable(),
            std::vector<std::shared_ptr<DescriptorSetLayout>>{this->descriptorSetLayout});
        this->noisePipeline = std::make_shared<ComputePipeline>(
            device, noiseShader->Renderable(),
            std::vector<std::shared_ptr<DescriptorSetLayout>>{this->noiseDescriptorSetLayout});
        this->sdfPipeline = std::make_shared<ComputePipeline>(
            device, sdfShader->Renderable(),
            std::vector<std::shared_ptr<DescriptorSetLayout>>{this->sdfDescriptorSetLayout});

        for (size_t i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++) {
            this->semaphores.emplace_back(
                std::move(Semaphore(Format("Cloud Renderer %i", i), device)));
        }

        this->noise = Engine::GetResources().CreateTexture(
            "Cloud Noise", nullptr, static_cast<unsigned>(NOISE_RESOLUTION),
            static_cast<unsigned>(NOISE_RESOLUTION), TextureType::UNORM, ImageDimensions::IMAGE_3D,
            static_cast<unsigned>(NOISE_RESOLUTION), false);
        this->sdf = Engine::GetResources().CreateTexture(
            "Cloud SDF", nullptr, static_cast<unsigned>(NOISE_RESOLUTION),
            static_cast<unsigned>(NOISE_RESOLUTION), TextureType::UNORM, ImageDimensions::IMAGE_3D,
            static_cast<unsigned>(NOISE_RESOLUTION), false);

        this->sampler = std::make_shared<Sampler>("Cloud Noise Sampler", device, physicalDevice);
        this->noiseDirty = true;
    }

    Semaphore& CloudRenderer::CurrentSemaphore(size_t currentFrame) {
        return this->semaphores[currentFrame];
    }

    void CloudRenderer::Render(CmdList& cmdList, std::shared_ptr<Image> colorImage,
                               std::shared_ptr<Image> depthImage, size_t currentFrame) {
        if (!this->config.enabled)
            return;

        if (this->noiseDirty) {
            this->GenerateNoise(cmdList);
            this->noiseDirty = false;
        }

        World& world = Engine::GetWorld();
        Camera& camera = world.camera;
        const glm::mat4& invViewMatrix = camera.transform.GetMatrix();
        const glm::mat4& invProjectionMatrix = glm::inverse(camera.GetMatrix());

        UniformBuffer uniformBuffer;
        uniformBuffer.invView = invViewMatrix;
        uniformBuffer.invProj = invProjectionMatrix;
        uniformBuffer.cloudOffset = glm::vec4(this->config.offset, 0.0);
        uniformBuffer.cloudScale = this->config.scale;
        uniformBuffer.cloudDensityThreshold = this->config.densityThreshold;
        uniformBuffer.cloudDensityMultiplier = this->config.densityMultiplier;
        uniformBuffer.lightDir = glm::vec4(world.sun.direction, 1.0);
        uniformBuffer.lightColor = glm::vec4(world.sun.lightSource.color, 1.0);
        uniformBuffer.forwardScattering = this->config.forwardScattering;
        uniformBuffer.backwardScattering = this->config.backwardScattering;
        uniformBuffer.baseBrightness = this->config.baseBrightness;
        uniformBuffer.phaseFactor = this->config.phaseFactor;
        uniformBuffer.lightAbsorbtion = this->config.lightAbsorbtion;
        uniformBuffer.indirectLightAbsorbtion = this->config.indirectLightAbsorbtion;
        uniformBuffer.darknessThreshold = this->config.darknessThreshold;
        uniformBuffer.zNear = camera.GetNear();
        uniformBuffer.zFar = camera.GetFar();
        void* uniformBufferData = this->uniformBuffers[currentFrame]->Map();
        memcpy(uniformBufferData, &uniformBuffer, sizeof(UniformBuffer));
        this->uniformBuffers[currentFrame]->Unmap();

        cmdList.BindComputePipeline(this->pipeline);

        cmdList.TransitionImageLayout(colorImage, ImageLayout::GENERAL,
                                      ResourceAccessBits::ACCESS_SHADER_READ_BIT |
                                          ResourceAccessBits::ACCESS_SHADER_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        cmdList.TransitionImageLayout(depthImage, ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                                      ResourceAccessBits::ACCESS_SHADER_READ_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);

        this->descriptorSets[currentFrame]->SetImage(0, DescriptorType::STORAGE_IMAGE, colorImage,
                                                     nullptr);
        this->descriptorSets[currentFrame]->SetImage(1, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                     depthImage, this->sampler);
        this->descriptorSets[currentFrame]->SetImage(2, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                     this->noise->Renderable().GetImage(),
                                                     this->sampler);
        cmdList.BindDescriptorSet(this->descriptorSets[currentFrame], 0, PipelineType::COMPUTE);
        cmdList.Dispatch(DivideUp(colorImage->Width(), 32), DivideUp(colorImage->Height(), 32));

        cmdList.TransitionImageLayout(
            colorImage, ImageLayout::PRESENT_SRC,
            ResourceAccessBits::ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            PipelineStageBits::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        cmdList.TransitionImageLayout(depthImage, ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                      ResourceAccessBits::ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
    }

    void CloudRenderer::GenerateNoise(CmdList& cmdList) {
        NoiseUniformBuffer noiseUniformBuffer;
        noiseUniformBuffer.size = static_cast<float>(NOISE_RESOLUTION);
        void* uniformBufferData = this->noiseUniformBuffer->Map();
        memcpy(uniformBufferData, &noiseUniformBuffer, sizeof(NoiseUniformBuffer));
        this->noiseUniformBuffer->Unmap();

        auto genNoiseImage = std::make_shared<Image>(
            "Cloud Noise Gen", this->device, this->physicalDevice,
            static_cast<uint32_t>(NOISE_RESOLUTION), static_cast<uint32_t>(NOISE_RESOLUTION), 1,
            ImageUsageBits::TRANSFER_SRC_IMAGE | ImageUsageBits::STORAGE_IMAGE,
            GraphicsFormat::R8G8B8A8_UNORM, 1, ImageDimensions::IMAGE_3D,
            static_cast<uint32_t>(NOISE_RESOLUTION));

        auto noiseImage = this->noise->Renderable().GetImage();

        cmdList.BindComputePipeline(this->noisePipeline);
        cmdList.TransitionImageLayout(genNoiseImage, ImageLayout::GENERAL,
                                      ResourceAccessBits::ACCESS_SHADER_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        this->noiseDescriptorSet->SetImage(0, DescriptorType::STORAGE_IMAGE, genNoiseImage,
                                           nullptr);
        cmdList.BindDescriptorSet(this->noiseDescriptorSet, 0, PipelineType::COMPUTE);
        cmdList.Dispatch(DivideUp(NOISE_RESOLUTION, 8), DivideUp(NOISE_RESOLUTION, 8),
                         DivideUp(NOISE_RESOLUTION, 8));

        cmdList.TransitionImageLayout(genNoiseImage, ImageLayout::TRANSFER_SRC_OPTIMAL,
                                      ResourceAccessBits::ACCESS_TRANSFER_READ_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_TRANSFER_BIT);
        cmdList.TransitionImageLayout(noiseImage, ImageLayout::TRANSFER_DST_OPTIMAL,
                                      ResourceAccessBits::ACCESS_TRANSFER_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_TRANSFER_BIT);
        cmdList.CopyImages(genNoiseImage, noiseImage);
        cmdList.TransitionImageLayout(noiseImage, ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                                      ResourceAccessBits::ACCESS_SHADER_READ_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);

        SDFUniformBuffer sdfUniformBuffer;
        sdfUniformBuffer.densityThreshold = this->config.densityThreshold;
        uniformBufferData = this->sdfUniformBuffer->Map();
        memcpy(uniformBufferData, &sdfUniformBuffer, sizeof(SDFUniformBuffer));
        this->sdfUniformBuffer->Unmap();

        auto genSDFImage = std::make_shared<Image>(
            "Cloud SDF Gen", this->device, this->physicalDevice,
            static_cast<uint32_t>(NOISE_RESOLUTION), static_cast<uint32_t>(NOISE_RESOLUTION), 1,
            ImageUsageBits::TRANSFER_SRC_IMAGE | ImageUsageBits::STORAGE_IMAGE,
            GraphicsFormat::R8G8B8A8_UNORM, 1, ImageDimensions::IMAGE_3D,
            static_cast<uint32_t>(NOISE_RESOLUTION));

        cmdList.BindComputePipeline(this->sdfPipeline);
        cmdList.TransitionImageLayout(genSDFImage, ImageLayout::GENERAL,
                                      ResourceAccessBits::ACCESS_SHADER_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        this->sdfDescriptorSet->SetImage(0, DescriptorType::COMBINED_IMAGE_SAMPLER, noiseImage,
                                           this->sampler);
        this->sdfDescriptorSet->SetImage(1, DescriptorType::STORAGE_IMAGE, genSDFImage, nullptr);
        cmdList.BindDescriptorSet(this->sdfDescriptorSet, 0, PipelineType::COMPUTE);
        cmdList.Dispatch(DivideUp(NOISE_RESOLUTION, 8), DivideUp(NOISE_RESOLUTION, 8),
                         DivideUp(NOISE_RESOLUTION, 8));
    }
}  // namespace UniverseEngine