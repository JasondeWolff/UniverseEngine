#include "Engine.h"

#include "Logging.h"

namespace UniverseEngine {
    Engine* Engine::gInstance = nullptr;

    Engine::Engine() : timer{}, frameCount(0) {
        UE_ASSERT_MSG(Engine::gInstance == nullptr, "Already an engine instance alive.");
        Engine::gInstance = this;

        this->resources = std::move(std::unique_ptr<Resources>(new Resources{}));
        this->world = std::move(std::unique_ptr<World>(new World{}));
        this->graphics = std::move(std::unique_ptr<Graphics>(new Graphics{}));
        this->input = std::move(std::unique_ptr<Input>(new Input{}));
    }

    void Engine::Quit() {
        GetGraphics().GetWindow().Close();
    }

    Resources& Engine::GetResources() {
        UE_ASSERT_MSG(Engine::gInstance && Engine::gInstance->resources.get(),
                      "Resources hasn't been initialized.");
        return *Engine::gInstance->resources.get();
    }

    World& Engine::GetWorld() {
        UE_ASSERT_MSG(Engine::gInstance && Engine::gInstance->world.get(),
                      "World hasn't been initialized.");
        return *Engine::gInstance->world.get();
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

    uint64_t Engine::FrameCount() {
        UE_ASSERT_MSG(Engine::gInstance,
                      "Engine hasn't been initialized.");
        return Engine::gInstance->frameCount;
    }

    void Engine::Run() {
        Timer fpsTimer{};
        unsigned fpsCount = 0;

        while (!GetGraphics().GetWindow().ShouldClose()) {
            if (fpsTimer.Elapsed() >= 1.0f) {
                UE_INFO("FPS %i", fpsCount);
                fpsCount = 0;
                fpsTimer.Reset();
            }
            fpsCount++;

            float deltaTime = this->timer.Elapsed();
            this->timer.Reset();

            this->game->Update(deltaTime);
            GetWorld().Update();
            GetInput().Update();
            GetGraphics().Update();
            GetResources().Update();
            
            frameCount++;
        }

        this->game->OnClose();
    }
}  // namespace UniverseEngine