#pragma once

#include <memory>

#include "Window.h"
#include "Resources.h"

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
        Handle<Shader> hShaderUnlit;
    };
}  // namespace UniverseEngine