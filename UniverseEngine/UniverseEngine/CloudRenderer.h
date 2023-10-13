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

        glm::vec3 weatherOffset = glm::vec3(0.0);
        glm::vec3 detailOffset = glm::vec3(0.0);
        glm::vec3 roughOffset = glm::vec3(0.0);
        float weatherScale = 0.15f;
        float detailScale = 0.46f;
        float roughScale = 0.35f;
        float weatherDensityThreshold = 0.18f;
        float detailDensityThreshold = 0.35f;
        float roughDensityThreshold = 0.39f;

        float densityMultiplier = 0.31f;

        float lightAbsorbtion = 1.0f;
        float eccentricity = 0.25f;
        float aoStrength = 0.3f;

        bool sdfDebug = false;
        float sdfFactor = 1.0f;
        float minTransmittance = 0.07f;
        RenderScale renderScale = RenderScale::QUARTER_RES;
    };

    class CloudRenderer {
    public:
        CloudRenderer(std::shared_ptr<LogicalDevice> device, const PhysicalDevice& physicalDevice,
                      std::shared_ptr<DescriptorPool> descriptorPool, const Graphics& graphics);

        Semaphore& CurrentSemaphore(size_t currentFrame);

        void Render(CmdList& cmdList, std::shared_ptr<Image> colorImage,
                    std::shared_ptr<Image> depthImage, size_t currentFrame);
        void BuildSizedResources(uint32_t width, uint32_t height);

        CloudConfig config;

    private:
        static const size_t NOISE_RESOLUTION = 64;

        const std::shared_ptr<LogicalDevice> device;
        const PhysicalDevice& physicalDevice;

        std::shared_ptr<ComputePipeline> pipeline;
        std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
        std::array<std::shared_ptr<Buffer>, Swapchain::MAX_FRAMES_IN_FLIGHT> uniformBuffers;
        std::array<std::shared_ptr<DescriptorSet>, Swapchain::MAX_FRAMES_IN_FLIGHT> descriptorSets;

        std::shared_ptr<ComputePipeline> compositPipeline;
        std::shared_ptr<DescriptorSetLayout> compositDescriptorSetLayout;
        std::shared_ptr<DescriptorSet> compositDescriptorSet;

        std::shared_ptr<ComputePipeline> noisePipeline;
        std::shared_ptr<DescriptorSetLayout> noiseDescriptorSetLayout;
        std::shared_ptr<Buffer> noiseUniformBuffer;
        std::shared_ptr<DescriptorSet> noiseDescriptorSet;

        std::shared_ptr<ComputePipeline> sdfPipeline;
        std::shared_ptr<DescriptorSetLayout> sdfDescriptorSetLayout;
        std::shared_ptr<Buffer> sdfUniformBuffer;
        std::shared_ptr<DescriptorSet> sdfDescriptorSet;

        std::shared_ptr<Image> downsizedRenderTarget;
        std::shared_ptr<Image> noise;
        std::shared_ptr<Image> sdf;
        std::shared_ptr<Sampler> sampler;

        std::vector<Semaphore> semaphores;

        CloudConfig oldConfig;

        void GenerateNoise(CmdList& cmdList);
    };
}  // namespace UniverseEngine