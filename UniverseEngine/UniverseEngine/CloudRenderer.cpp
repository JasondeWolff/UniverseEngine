#include "CloudRenderer.h"

#include <FastNoise/FastNoise.h>

#include "Engine.h"
#include "Format.h"

struct UniformBuffer {
    glm::mat4 invView;
    glm::mat4 invProj;
};

namespace UniverseEngine {
    CloudRenderer::CloudRenderer(std::shared_ptr<LogicalDevice> device,
                                 const PhysicalDevice& physicalDevice,
                                 std::shared_ptr<DescriptorPool> descriptorPool,
                                 const Graphics& graphics) {
        this->descriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            device, std::vector<DescriptorLayoutBinding>{
                        DescriptorLayoutBinding("outputImage", 0, DescriptorType::STORAGE_IMAGE,
                                                GraphicsStageFlagBits::COMPUTE_STAGE),
                        DescriptorLayoutBinding("ubo", 1, DescriptorType::UNIFORM_BUFFER,
                                                GraphicsStageFlagBits::COMPUTE_STAGE),
                        DescriptorLayoutBinding("noise", 2, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                GraphicsStageFlagBits::COMPUTE_STAGE)});

        for (size_t i = 0; i < this->descriptorSets.size(); i++) {
            this->uniformBuffers[i] = std::make_shared<Buffer>(
                Format("Cloud UniformBuffer_%i", i), device, physicalDevice,
                BufferUsageBits::UNIFORM_BUFFER, sizeof(UniformBuffer), BufferLocation::CPU_TO_GPU);
            this->descriptorSets[i] =
                std::make_shared<DescriptorSet>(device, descriptorPool, this->descriptorSetLayout);

            this->descriptorSets[i]->SetBuffer(1, DescriptorType::UNIFORM_BUFFER,
                                               this->uniformBuffers[i]);
        }

        auto& resources = Engine::GetResources();
        std::shared_ptr<Shader> shader = resources.LoadShader("Assets/Shaders/clouds.comp");
        graphics.RebuildShaders();

        this->pipeline = std::make_shared<ComputePipeline>(
            device, shader->Renderable(),
            std::vector<std::shared_ptr<DescriptorSetLayout>>{this->descriptorSetLayout});

        for (size_t i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++) {
            this->semaphores.emplace_back(
                std::move(Semaphore(Format("Cloud Renderer %i", i), device)));
        }

        this->sampler = std::make_shared<Sampler>("Cloud Noise Sampler", device, physicalDevice);

        this->GenerateNoise();
    }

    Semaphore& CloudRenderer::CurrentSemaphore(size_t currentFrame) {
        return this->semaphores[currentFrame];
    }

    void CloudRenderer::Render(CmdList& cmdList, std::shared_ptr<Image> colorImage,
                               size_t currentFrame) {
        World& world = Engine::GetWorld();
        Camera& camera = world.camera;
        const glm::mat4& invViewMatrix = camera.transform.GetMatrix();
        const glm::mat4& invProjectionMatrix = glm::inverse(camera.GetMatrix());

        UniformBuffer uniformBuffer{invViewMatrix, invProjectionMatrix};
        void* uniformBufferData = this->uniformBuffers[currentFrame]->Map();
        memcpy(uniformBufferData, &uniformBuffer, sizeof(UniformBuffer));
        this->uniformBuffers[currentFrame]->Unmap();

        cmdList.BindComputePipeline(this->pipeline);

        cmdList.TransitionImageLayout(colorImage, ImageLayout::PRESENT_SRC, ImageLayout::GENERAL);

        this->descriptorSets[currentFrame]->SetImage(0, DescriptorType::STORAGE_IMAGE, colorImage,
                                                     nullptr);
        this->descriptorSets[currentFrame]->SetImage(2, DescriptorType::COMBINED_IMAGE_SAMPLER,
                                                     this->noise->Renderable().GetImage(), this->sampler);
        cmdList.BindDescriptorSet(this->descriptorSets[currentFrame], 0, PipelineType::COMPUTE);
        cmdList.Dispatch(DivideUp(colorImage->Width(), 16), DivideUp(colorImage->Height(), 16));

        cmdList.TransitionImageLayout(colorImage, ImageLayout::GENERAL, ImageLayout::PRESENT_SRC);
    }

    void CloudRenderer::GenerateNoise() {
        size_t resolution = 256;

        auto fnCellular = FastNoise::New<FastNoise::CellularDistance>();
        auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();
        fnFractal->SetSource(fnCellular);
        fnFractal->SetOctaveCount(1);

        float* noiseData =
            static_cast<float*>(malloc(resolution * resolution * resolution * sizeof(float)));
        fnFractal->GenUniformGrid3D(noiseData, 0, 0, 0, static_cast<int>(resolution),
                                    static_cast<int>(resolution), static_cast<int>(resolution),
                                    0.2f, 1337);

        unsigned char* noiseData8 = static_cast<unsigned char*>(
            malloc(resolution * resolution * resolution * sizeof(unsigned char) * 4));
        for (size_t i = 0; i < resolution * resolution * resolution; i++) {
            noiseData8[i * 4 + 0] = static_cast<unsigned char>(noiseData[i] * 255.99f);
            noiseData8[i * 4 + 1] = static_cast<unsigned char>(noiseData[i] * 255.99f);
            noiseData8[i * 4 + 2] = static_cast<unsigned char>(noiseData[i] * 255.99f);
            noiseData8[i * 4 + 3] = static_cast<unsigned char>(noiseData[i] * 255.99f);
        }

        this->noise = Engine::GetResources().CreateTexture(
            "Cloud Noise", noiseData8, static_cast<unsigned>(resolution),
            static_cast<unsigned>(resolution), TextureType::UNORM, ImageDimensions::IMAGE_3D,
            static_cast<unsigned>(resolution), false);
    }
}  // namespace UniverseEngine