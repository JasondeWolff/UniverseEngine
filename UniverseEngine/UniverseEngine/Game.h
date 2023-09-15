#pragma once

namespace UniverseEngine {
    class Game {
    public:
        Game() = default;

        virtual void OnStart(){};
        virtual void Update(float deltaTime){};
        virtual void OnClose(){};
    };
}  // namespace UniverseEngine