#pragma once

#include <memory>

#include "CmdList.h"
#include "GraphicsPipeline.h"
#include "Resources.h"
#include "Window.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"

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
        std::unique_ptr<GraphicsInstance> instance;
        std::unique_ptr<PhysicalDevice> physicalDevice;
        std::unique_ptr<LogicalDevice> device;

        std::shared_ptr<GraphicsPipeline> unlitPipeline;
    };
}  // namespace UniverseEngine