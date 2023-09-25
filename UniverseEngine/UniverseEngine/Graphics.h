#pragma once

#include <memory>

#include "CmdList.h"
#include "CmdQueue.h"
#include "DescriptorLayoutBinding.h"
#include "DescriptorPool.h"
#include "DescriptorSet.h"
#include "DescriptorSetLayout.h"
#include "GraphicsPipeline.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "PushConstantRange.h"
#include "RenderPass.h"
#include "Resources.h"
#include "Sampler.h"
#include "Swapchain.h"
#include "Window.h"

namespace UniverseEngine {
    class Engine;

    class Graphics {
    public:
        ~Graphics();

        const Window& GetWindow() const;

    private:
        friend class Engine;
        Graphics();

        void Update();
        void Render();

        void BuildSwapchain();
        void BuildPipelines();
        void BuildRenderables();

        std::unique_ptr<Window> window;
        std::shared_ptr<GraphicsInstance> instance;
        std::unique_ptr<PhysicalDevice> physicalDevice;
        std::shared_ptr<LogicalDevice> device;
        std::unique_ptr<Swapchain> swapchain;
        std::shared_ptr<Image> depthImage;
        std::unique_ptr<CmdQueue> cmdQueue;
        std::shared_ptr<DescriptorPool> descriptorPool;

        std::shared_ptr<RenderPass> renderPass;
        std::shared_ptr<GraphicsPipeline> unlitPipeline;

        std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
        std::array<std::shared_ptr<Buffer>, Swapchain::MAX_FRAMES_IN_FLIGHT> uniformBuffers;
        std::array<std::shared_ptr<DescriptorSet>, Swapchain::MAX_FRAMES_IN_FLIGHT> descriptorSets;

        std::shared_ptr<Sampler> sampler;
    };
}  // namespace UniverseEngine