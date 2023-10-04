#include "CloudRenderer.h"

#include "Engine.h"
#include "Format.h"

namespace UniverseEngine {
    CloudRenderer::CloudRenderer(std::shared_ptr<LogicalDevice> device,
                                 const PhysicalDevice& physicalDevice,
                                 std::shared_ptr<DescriptorPool> descriptorPool,
                                 const Graphics& graphics) {
        this->descriptorSetLayout = std::make_shared<DescriptorSetLayout>(
            device, std::vector<DescriptorLayoutBinding>{
                        DescriptorLayoutBinding("outputImage", 0, DescriptorType::STORAGE_IMAGE,
                                                GraphicsStageFlagBits::COMPUTE_STAGE)});

        for (size_t i = 0; i < this->descriptorSets.size(); i++) {
            this->descriptorSets[i] =
                std::make_shared<DescriptorSet>(device, descriptorPool, this->descriptorSetLayout);
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
    }

    Semaphore& CloudRenderer::CurrentSemaphore(size_t currentFrame) {
        return this->semaphores[currentFrame];
    }

    void CloudRenderer::Render(CmdList& cmdList, std::shared_ptr<Image> colorImage,
                               size_t currentFrame) {
        cmdList.BindComputePipeline(this->pipeline);

        cmdList.TransitionImageLayout(colorImage, ImageLayout::PRESENT_SRC, ImageLayout::GENERAL);

        this->descriptorSets[currentFrame]->SetImage(0, DescriptorType::STORAGE_IMAGE, colorImage,
                                                     nullptr);
        cmdList.BindDescriptorSet(this->descriptorSets[currentFrame], 0, PipelineType::COMPUTE);
        cmdList.Dispatch(DivideUp(colorImage->Width(), 16), DivideUp(colorImage->Height(), 16));

        cmdList.TransitionImageLayout(colorImage, ImageLayout::GENERAL, ImageLayout::PRESENT_SRC);
    }
}  // namespace UniverseEngine