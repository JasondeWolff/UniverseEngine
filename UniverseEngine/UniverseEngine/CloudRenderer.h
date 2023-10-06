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
        bool enabled = false;

        glm::vec3 offset = glm::vec3(0.0);
        float scale = 0.6f;
        float densityThreshold = 0.2f;
        float densityMultiplier = 1.5f;
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
        std::shared_ptr<ComputePipeline> pipeline;
        std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
        std::array<std::shared_ptr<Buffer>, Swapchain::MAX_FRAMES_IN_FLIGHT> uniformBuffers;
        std::array<std::shared_ptr<DescriptorSet>, Swapchain::MAX_FRAMES_IN_FLIGHT> descriptorSets;

        std::shared_ptr<Texture> noise;
        std::shared_ptr<Sampler> sampler;

        std::vector<Semaphore> semaphores;

        void GenerateNoise();
    };
}  // namespace UniverseEngine