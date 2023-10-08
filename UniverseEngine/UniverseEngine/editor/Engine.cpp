#include "../Defines.h"
#ifdef EDITOR

#include "../Engine.h"

namespace UniverseEngine {
    void Engine::Editor() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Game")) {
                this->game->EditorBar();
            }
            if (ImGui::BeginMenu("Graphics")) {
                GetGraphics().EditorBar();
            }
            if (ImGui::BeginMenu("Input")) {
                GetInput().EditorBar();
            }
            ImGui::EndMainMenuBar();
        }

        this->game->Editor();
        GetGraphics().Editor();
        GetInput().Editor();
    }
}
#endif