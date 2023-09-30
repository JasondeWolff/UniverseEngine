#pragma once

#include <imgui.h>

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
    class Graphics;

    class ImGuiRenderer {
    public:
        ImGuiRenderer(std::shared_ptr<LogicalDevice> device, const PhysicalDevice& physicalDevice,
                      std::shared_ptr<DescriptorPool> descriptorPool,
                      std::shared_ptr<RenderPass> renderPass, const Graphics& graphics);

        void Render(CmdList& cmdList, size_t currentFrame);

        const static size_t MAX_VERTICES = 1024 * 8;
        const static size_t MAX_INDICES = 1024 * 16;

    private:
        std::shared_ptr<GraphicsPipeline> pipeline;
        std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
        std::array<std::shared_ptr<DescriptorSet>, Swapchain::MAX_FRAMES_IN_FLIGHT> descriptorSets;
        std::shared_ptr<Sampler> sampler;
        std::shared_ptr<Texture> fontTexture;

        std::shared_ptr<Buffer> vertexBuffer;
        std::shared_ptr<Buffer> indexBuffer;
    };
}  // namespace UniverseEngine