#pragma once

#include <imgui.h>

#include <memory>

#include "CmdList.h"
#include "CmdQueue.h"
#include "DescriptorLayoutBinding.h"
#include "DescriptorPool.h"
#include "DescriptorSet.h"
#include "DescriptorSetLayout.h"
#include "ComputePipeline.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "PushConstantRange.h"
#include "RenderPass.h"
#include "Resources.h"
#include "Sampler.h"
#include "Swapchain.h"
#include "Window.h"

namespace UniverseEngine {
    class Graphics;

    struct CloudConfig {
        bool enabled = true;

        glm::vec3 offset = glm::vec3(0.0);
        float scale = 0.263f;
        float densityThreshold = 0.69f;
        float densityMultiplier = 1.5f;

        float forwardScattering = 0.72f;
        float backwardScattering = 0.33f;
        float baseBrightness = 0.7f;
        float phaseFactor = 0.74f;

        float lightAbsorbtion = 0.8f;
        float indirectLightAbsorbtion = 0.96f;
        float darknessThreshold = 0.12f;
    };

    class CloudRenderer {
    public:
        CloudRenderer(std::shared_ptr<LogicalDevice> device, const PhysicalDevice& physicalDevice,
                      std::shared_ptr<DescriptorPool> descriptorPool, const Graphics& graphics);

        Semaphore& CurrentSemaphore(size_t currentFrame);

        void Render(CmdList& cmdList, std::shared_ptr<Image> colorImage,
                    std::shared_ptr<Image> depthImage, size_t currentFrame);

        CloudConfig config;

    private:
        static const size_t NOISE_RESOLUTION = 64;

        const std::shared_ptr<LogicalDevice> device;
        const PhysicalDevice& physicalDevice;

        std::shared_ptr<ComputePipeline> pipeline;
        std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
        std::array<std::shared_ptr<Buffer>, Swapchain::MAX_FRAMES_IN_FLIGHT> uniformBuffers;
        std::array<std::shared_ptr<DescriptorSet>, Swapchain::MAX_FRAMES_IN_FLIGHT> descriptorSets;

        std::shared_ptr<ComputePipeline> noisePipeline;
        std::shared_ptr<DescriptorSetLayout> noiseDescriptorSetLayout;
        std::shared_ptr<Buffer> noiseUniformBuffer;
        std::shared_ptr<DescriptorSet> noiseDescriptorSet;

        std::shared_ptr<ComputePipeline> sdfPipeline;
        std::shared_ptr<DescriptorSetLayout> sdfDescriptorSetLayout;
        std::shared_ptr<Buffer> sdfUniformBuffer;
        std::shared_ptr<DescriptorSet> sdfDescriptorSet;

        bool noiseDirty;
        std::shared_ptr<Image> noise;
        std::shared_ptr<Image> sdf;
        std::shared_ptr<Sampler> sampler;

        std::vector<Semaphore> semaphores;

        CloudConfig oldConfig;

        void GenerateNoise(CmdList& cmdList);
    };
}  // namespace UniverseEngine