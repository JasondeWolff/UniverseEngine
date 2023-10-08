#pragma once

#include <imgui.h>

namespace UniverseEngine {
    class Game {
    public:
        Game() = default;

        virtual void OnStart(){};
        virtual void Update(float deltaTime){};
        virtual void OnClose(){};

        virtual void EditorBar() {
            ImGui::EndMenu();
        };
        virtual void Editor(){};
    };
}  // namespace UniverseEngine