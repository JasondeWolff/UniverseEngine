#pragma once

#include <memory>
#include <type_traits>

#include "Game.h"
#include "Graphics.h"
#include "Input.h"
#include "Resources.h"
#include "Timer.h"
#include "World.h"

namespace UniverseEngine {
    class Engine {
    public:
        Engine();
        template <typename T>
        void Start();

        static void Quit();

        static Resources& GetResources();
        static World& GetWorld();
        static Graphics& GetGraphics();
        static Input& GetInput();

        static uint64_t FrameCount();

    private:
        static Engine* gInstance;

        Timer timer;
        uint64_t frameCount;
        std::unique_ptr<Game> game;

        std::unique_ptr<Resources> resources;
        std::unique_ptr<World> world;
        std::unique_ptr<Graphics> graphics;
        std::unique_ptr<Input> input;

        void Run();
    };

    template<typename T>
    void Engine::Start() {
        static_assert(std::is_base_of<Game, T>::value, "T must derive from Game.");
        UE_ASSERT_MSG(this->game == nullptr, "Game is already running.");

        this->game = std::make_unique<T>();
        this->game->OnStart();
        this->Run();
    }
}  // namespace UniverseEngine