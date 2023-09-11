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

        void Run();

    private:
        static Engine* gInstance;

        std::unique_ptr<Resources> resources;
        std::unique_ptr<Graphics> graphics;
        std::unique_ptr<Input> input;
    };
}  // namespace UniverseEngine