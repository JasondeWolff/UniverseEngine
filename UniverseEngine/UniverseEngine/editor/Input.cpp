#include "../Defines.h"
#ifdef EDITOR

#include <imgui.h>

#include "../Input.h"

namespace UniverseEngine {
    static bool general = false;

    void Input::EditorBar() {
        if (ImGui::MenuItem("General")) {
            general = true;
        }
        ImGui::EndMenu();
    }

    void Input::Editor() {
        if (general) {
            ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Appearing);
            ImGui::Begin("[INPUT] General", &general);
            static float sensitivity = 1.0f;
            ImGui::SliderFloat("Mouse Sensitivity", &sensitivity, 0.0f, 3.0f);
            ImGui::End();
        }
    }
}  // namespace UniverseEngine
#endif