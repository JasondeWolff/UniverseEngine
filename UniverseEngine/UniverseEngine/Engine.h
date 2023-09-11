#pragma once

#include <memory>

#include "Graphics.h"
#include "Input.h"
#include "Resources.h"

namespace UniverseEngine {
    class Engine {
    public:
        Engine();

        static void Quit();

        static Resources& GetResources();
        static Graphics& GetGraphics();
        static Input& GetInput();

    private:
        static Engine* gInstance;

        void Run();

        std::unique_ptr<Resources> resources;
        std::unique_ptr<Graphics> graphics;
        std::unique_ptr<Input> input;
    };
}  // namespace UniverseEngine