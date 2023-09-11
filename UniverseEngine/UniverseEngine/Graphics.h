#pragma once

#include <memory>

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

        std::unique_ptr<Window> window;
    };
}  // namespace UniverseEngine