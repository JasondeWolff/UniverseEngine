#include "Engine.h"

#include "Logging.h"

namespace UniverseEngine {
    Engine* Engine::gInstance = nullptr;

    Engine::Engine() {
        UE_ASSERT_MSG(Engine::gInstance == nullptr, "Already an engine instance alive.");
        Engine::gInstance = this;

        this->resources = std::move(std::unique_ptr<Resources>(new Resources{}));
        this->graphics = std::move(std::unique_ptr<Graphics>(new Graphics{}));
        this->input = std::move(std::unique_ptr<Input>(new Input{}));

        Run();
    }

    void Engine::Quit() {
        GetGraphics().GetWindow().Close();
    }

    Resources& Engine::GetResources() {
        UE_ASSERT_MSG(Engine::gInstance && Engine::gInstance->resources.get(),
                      "Resources hasn't been initialized.");
        return *Engine::gInstance->resources.get();
    }

    Graphics& Engine::GetGraphics() {
        UE_ASSERT_MSG(Engine::gInstance && Engine::gInstance->graphics.get(),
                      "Graphics hasn't been initialized.");
        return *Engine::gInstance->graphics.get();
    }

    Input& Engine::GetInput() {
        UE_ASSERT_MSG(Engine::gInstance && Engine::gInstance->input.get(),
                      "Input hasn't been initialized.");
        return *Engine::gInstance->input.get();
    }

    void Engine::Run() {
        while (!GetGraphics().GetWindow().ShouldClose()) {
            GetGraphics().Update();
        }
    }
}  // namespace UniverseEngine