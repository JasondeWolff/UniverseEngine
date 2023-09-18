#pragma once

#include <memory>

#include "CmdList.h"
#include "CmdQueue.h"
#include "GraphicsPipeline.h"
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include "RenderPass.h"
#include "Resources.h"
#include "Swapchain.h"
#include "Window.h"

namespace UniverseEngine {
    class Engine;

    class Graphics {
    public:
        const Window& GetWindow() const;

    private:
        friend class Engine;
        Graphics();

        void Update();

        void BuildRenderables();

        std::unique_ptr<Window> window;
        std::shared_ptr<GraphicsInstance> instance;
        std::unique_ptr<PhysicalDevice> physicalDevice;
        std::shared_ptr<LogicalDevice> device;
        std::unique_ptr<Swapchain> swapchain;
        std::unique_ptr<CmdQueue> cmdQueue;

        std::shared_ptr<RenderPass> renderPass;
        std::shared_ptr<GraphicsPipeline> unlitPipeline;
    };
}  // namespace UniverseEngine