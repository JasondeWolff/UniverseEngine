#pragma once

#include <memory>

#include "GraphicsPipeline.h"
#include "Resources.h"
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
        std::unique_ptr<GraphicsPipeline> unlitPipeline;
    };
}  // namespace UniverseEngine