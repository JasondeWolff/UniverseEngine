#include "CloudRenderer.h"

#include <FastNoise/FastNoise.h>

#include "Engine.h"
#include "Format.h"

struct UniformBuffer {
    glm::mat4 invView;
    glm::mat4 invProj;

    glm::vec4 lightDir;
    glm::vec4 lightColor;

    glm::vec4 weatherOffset;
    glm::vec4 detailOffset;
    glm::vec4 roughOffset;
    float weatherScale;
    float detailScale;
    float roughScale;
    float weatherDensityThreshold;
    float detailDensityThreshold;
    float roughDensityThreshold;

    float cloudDensityMultiplier;

    float lightAbsorbtion;
    float eccentricity;
    float aoStrength;

    float zNear;
    float zFar;

    int sdfDebug;
    float sdfFactor;
    float minTransmittance;
    float blueNoiseStrength;
    float maxDepth;

    float PADDING[3];
};

struct NoiseUniformBuffer {
    float size;
};

struct SDFUniformBuffer {
    float densityThreshold;
    float densityMultiplier;
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
                DescriptorLayoutBinding("sdf", 3, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                        GraphicsStageFlagBits::COMPUTE_STAGE),
                DescriptorLayoutBinding("blueNoise", 4, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                        GraphicsStageFlagBits::COMPUTE_STAGE),
                DescriptorLayoutBinding("ubo", 5, DescriptorType::UNIFORM_BUFFER,
                                        GraphicsStageFlagBits::COMPUTE_STAGE)});
        this->compositDescriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            device, std::vector<DescriptorLayoutBinding>{
                        DescriptorLayoutBinding("clouds", 0, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                GraphicsStageFlagBits::COMPUTE_STAGE),
                        DescriptorLayoutBinding("outputImage", 1, DescriptorType::STORAGE_IMAGE,
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

            this->descriptorSets[i]->SetBuffer(5, DescriptorType::UNIFORM_BUFFER,
                                               this->uniformBuffers[i]);
        }

        this->compositDescriptorSet = std::make_shared<DescriptorSet>(
            device, descriptorPool, this->compositDescriptorSetLayout);

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
        std::shared_ptr<Shader> compositShader =
            resources.LoadShader("Assets/Shaders/cloudComposit.comp");
        std::shared_ptr<Shader> noiseShader =
            resources.LoadShader("Assets/Shaders/cloudNoise.comp");
        std::shared_ptr<Shader> sdfShader = resources.LoadShader("Assets/Shaders/cloudSDF.comp");
        graphics.RebuildShaders();

        this->pipeline = std::make_shared<ComputePipeline>(
            device, shader->Renderable(),
            std::vector<std::shared_ptr<DescriptorSetLayout>>{this->descriptorSetLayout});
        this->compositPipeline = std::make_shared<ComputePipeline>(
            device, compositShader->Renderable(),
            std::vector<std::shared_ptr<DescriptorSetLayout>>{this->compositDescriptorSetLayout});
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

        unsigned mips = static_cast<unsigned>(std::floor(std::log2(NOISE_RESOLUTION))) + 1;
        this->noise = std::make_shared<Image>(
            "Cloud Noise", device, physicalDevice, static_cast<unsigned>(NOISE_RESOLUTION),
            static_cast<unsigned>(NOISE_RESOLUTION), mips,
            ImageUsageBits::TRANSFER_SRC_IMAGE | ImageUsageBits::TRANSFER_DST_IMAGE |
                ImageUsageBits::SAMPLED_IMAGE,
            GraphicsFormat::R8G8B8A8_UNORM, 1, ImageDimensions::IMAGE_3D,
            static_cast<unsigned>(NOISE_RESOLUTION));
        this->sdf = std::make_shared<Image>(
            "Cloud SDF", device, physicalDevice, static_cast<unsigned>(NOISE_RESOLUTION),
            static_cast<unsigned>(NOISE_RESOLUTION), 1,
            ImageUsageBits::TRANSFER_SRC_IMAGE | ImageUsageBits::TRANSFER_DST_IMAGE |
                ImageUsageBits::SAMPLED_IMAGE,
            GraphicsFormat::R8_UNORM, 1, ImageDimensions::IMAGE_3D,
            static_cast<unsigned>(NOISE_RESOLUTION));
        this->blueNoise =
            Engine::GetResources().LoadTexture("Assets/BlueNoise.png", TextureType::UNORM);

        SamplerInfo samplerInfo;
        samplerInfo.filterMode = FilterMode::NEAREST;
        this->nearestSampler =
            std::make_shared<Sampler>("Cloud Noise Sampler", device, physicalDevice, samplerInfo);
        samplerInfo.filterMode = FilterMode::LINEAR;
        this->sampler =
            std::make_shared<Sampler>("Cloud Noise Sampler", device, physicalDevice, samplerInfo);

        this->oldConfig.weatherDensityThreshold = -1.0f;
    }

    Semaphore& CloudRenderer::CurrentSemaphore(size_t currentFrame) {
        return this->semaphores[currentFrame];
    }

    void CloudRenderer::Render(CmdList& cmdList, std::shared_ptr<Image> colorImage,
                               std::shared_ptr<Image> depthImage, size_t currentFrame) {
        if (!this->config.enabled)
            return;

        if (this->config.weatherDensityThreshold != this->oldConfig.weatherDensityThreshold) {
            this->GenerateNoise(cmdList);
        }
        if (this->config.renderScale != this->oldConfig.renderScale) {
            this->BuildSizedResources(colorImage->Width(), colorImage->Height());
        }
        this->oldConfig = this->config;

        World& world = Engine::GetWorld();
        Camera& camera = world.camera;
        const glm::mat4& invViewMatrix = camera.transform.GetMatrix();
        const glm::mat4& invProjectionMatrix = glm::inverse(camera.GetMatrix());

        UniformBuffer uniformBuffer;
        uniformBuffer.invView = invViewMatrix;
        uniformBuffer.invProj = invProjectionMatrix;
        uniformBuffer.weatherOffset = glm::vec4(this->config.weatherOffset, 0.0);
        uniformBuffer.detailOffset = glm::vec4(this->config.detailOffset, 0.0);
        uniformBuffer.roughOffset = glm::vec4(this->config.roughOffset, 0.0);
        uniformBuffer.weatherScale = this->config.weatherScale;
        uniformBuffer.detailScale = this->config.detailScale;
        uniformBuffer.roughScale = this->config.roughScale;
        uniformBuffer.weatherDensityThreshold = this->config.weatherDensityThreshold;
        uniformBuffer.detailDensityThreshold = this->config.detailDensityThreshold;
        uniformBuffer.roughDensityThreshold = this->config.roughDensityThreshold;
        uniformBuffer.cloudDensityMultiplier = this->config.densityMultiplier;
        uniformBuffer.lightDir = glm::vec4(world.sun.direction, 1.0);
        uniformBuffer.lightColor = glm::vec4(world.sun.lightSource.color, 1.0);
        uniformBuffer.lightAbsorbtion = this->config.lightAbsorbtion;
        uniformBuffer.eccentricity = this->config.eccentricity;
        uniformBuffer.aoStrength = this->config.aoStrength;
        uniformBuffer.zNear = camera.GetNear();
        uniformBuffer.zFar = camera.GetFar();
        uniformBuffer.sdfDebug = this->config.sdfDebug;
        uniformBuffer.sdfFactor = this->config.sdfFactor;
        uniformBuffer.minTransmittance = this->config.minTransmittance;
        uniformBuffer.blueNoiseStrength = this->config.blueNoiseStrength;
        uniformBuffer.maxDepth = this->config.maxDepth;
        void* uniformBufferData = this->uniformBuffers[currentFrame]->Map();
        memcpy(uniformBufferData, &uniformBuffer, sizeof(UniformBuffer));
        this->uniformBuffers[currentFrame]->Unmap();

        // Render clouds at lower resolution
        cmdList.BindComputePipeline(this->pipeline);
        cmdList.TransitionImageLayout(this->downsizedRenderTarget, ImageLayout::GENERAL,
                                      ResourceAccessBits::ACCESS_SHADER_READ_BIT |
                                          ResourceAccessBits::ACCESS_SHADER_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        cmdList.TransitionImageLayout(depthImage, ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                                      ResourceAccessBits::ACCESS_SHADER_READ_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        this->descriptorSets[currentFrame]->SetImage(0, DescriptorType::STORAGE_IMAGE,
                                                     this->downsizedRenderTarget, nullptr);
        this->descriptorSets[currentFrame]->SetImage(1, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                     depthImage, this->nearestSampler);
        this->descriptorSets[currentFrame]->SetImage(2, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                     this->noise, this->sampler);
        this->descriptorSets[currentFrame]->SetImage(3, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                     this->sdf, this->sampler);
        this->descriptorSets[currentFrame]->SetImage(4, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                     this->blueNoise->Renderable().GetImage(),
                                                     this->sampler);
        cmdList.BindDescriptorSet(this->descriptorSets[currentFrame], 0, PipelineType::COMPUTE);
        cmdList.Dispatch(DivideUp(this->downsizedRenderTarget->Width(), 32),
                         DivideUp(this->downsizedRenderTarget->Height(), 32));

        // Composit clouds onto the full res color image
        cmdList.BindComputePipeline(this->compositPipeline);
        cmdList.TransitionImageLayout(this->downsizedRenderTarget,
                                      ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                                      ResourceAccessBits::ACCESS_SHADER_READ_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        cmdList.TransitionImageLayout(colorImage, ImageLayout::GENERAL,
                                      ResourceAccessBits::ACCESS_SHADER_READ_BIT |
                                          ResourceAccessBits::ACCESS_SHADER_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        this->compositDescriptorSet->SetImage(0, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                              this->downsizedRenderTarget, this->sampler);
        this->compositDescriptorSet->SetImage(1, DescriptorType::STORAGE_IMAGE, colorImage,
                                              nullptr);
        cmdList.BindDescriptorSet(this->compositDescriptorSet, 0, PipelineType::COMPUTE);
        cmdList.Dispatch(DivideUp(colorImage->Width(), 32), DivideUp(colorImage->Height(), 32));

        // Transition color and depth back into attachment states
        cmdList.TransitionImageLayout(
            colorImage, ImageLayout::PRESENT_SRC,
            ResourceAccessBits::ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            PipelineStageBits::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        cmdList.TransitionImageLayout(depthImage, ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                      ResourceAccessBits::ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT);
    }

    void CloudRenderer::BuildSizedResources(uint32_t width, uint32_t height) {
        switch (this->config.renderScale) {
            case RenderScale::HALF_RES: {
                width /= 2;
                height /= 2;
                break;
            }
            case RenderScale::QUARTER_RES: {
                width /= 4;
                height /= 4;
                break;
            }
        }

        this->downsizedRenderTarget = std::make_shared<Image>(
            "Cloud Render Target", this->device, this->physicalDevice, width, height, 1,
            ImageUsageBits::COLOR_ATTACHMENT | ImageUsageBits::SAMPLED_IMAGE |
                ImageUsageBits::STORAGE_IMAGE,
            GraphicsFormat::R16G16B16A16_SFLOAT);
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
        cmdList.TransitionImageLayout(this->noise, ImageLayout::TRANSFER_DST_OPTIMAL,
                                      ResourceAccessBits::ACCESS_TRANSFER_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_TRANSFER_BIT);
        cmdList.CopyImages(genNoiseImage, this->noise);
        cmdList.GenerateMips(this->noise);

        SDFUniformBuffer sdfUniformBuffer;
        sdfUniformBuffer.densityThreshold = this->config.weatherDensityThreshold;
        sdfUniformBuffer.densityMultiplier = this->config.densityMultiplier;
        uniformBufferData = this->sdfUniformBuffer->Map();
        memcpy(uniformBufferData, &sdfUniformBuffer, sizeof(SDFUniformBuffer));
        this->sdfUniformBuffer->Unmap();

        auto genSDFImage = std::make_shared<Image>(
            "Cloud SDF Gen", this->device, this->physicalDevice,
            static_cast<uint32_t>(NOISE_RESOLUTION), static_cast<uint32_t>(NOISE_RESOLUTION), 1,
            ImageUsageBits::TRANSFER_SRC_IMAGE | ImageUsageBits::STORAGE_IMAGE,
            GraphicsFormat::R8_UNORM, 1, ImageDimensions::IMAGE_3D,
            static_cast<uint32_t>(NOISE_RESOLUTION));

        cmdList.BindComputePipeline(this->sdfPipeline);
        cmdList.TransitionImageLayout(genSDFImage, ImageLayout::GENERAL,
                                      ResourceAccessBits::ACCESS_SHADER_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        cmdList.TransitionImageLayout(this->noise, ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                                      ResourceAccessBits::ACCESS_SHADER_READ_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        this->sdfDescriptorSet->SetImage(0, DescriptorType::COMBINED_IMAGE_SAMPLER, this->noise,
                                         this->sampler);
        this->sdfDescriptorSet->SetImage(1, DescriptorType::STORAGE_IMAGE, genSDFImage, nullptr);
        cmdList.BindDescriptorSet(this->sdfDescriptorSet, 0, PipelineType::COMPUTE);
        cmdList.Dispatch(DivideUp(NOISE_RESOLUTION, 8), DivideUp(NOISE_RESOLUTION, 8),
                         DivideUp(NOISE_RESOLUTION, 8));

        cmdList.TransitionImageLayout(genSDFImage, ImageLayout::TRANSFER_SRC_OPTIMAL,
                                      ResourceAccessBits::ACCESS_TRANSFER_READ_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_TRANSFER_BIT);
        cmdList.TransitionImageLayout(this->sdf, ImageLayout::TRANSFER_DST_OPTIMAL,
                                      ResourceAccessBits::ACCESS_TRANSFER_WRITE_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_TRANSFER_BIT);
        cmdList.CopyImages(genSDFImage, this->sdf);

        cmdList.TransitionImageLayout(this->sdf, ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                                      ResourceAccessBits::ACCESS_SHADER_READ_BIT,
                                      PipelineStageBits::PIPELINE_STAGE_COMPUTE_SHADER_BIT);
    }
}  // namespace UniverseEngine