#include "../Defines.h"
#ifdef EDITOR

#include "../Graphics.h"

namespace UniverseEngine {
    static bool general = false;
    static bool clouds = false;
    static bool tonemapping = false;

    void Graphics::EditorBar() {
        if (ImGui::MenuItem("General")) {
            general = true;
        }
        if (ImGui::MenuItem("Clouds")) {
            clouds = true;
        }
        if (ImGui::MenuItem("Tonemapping")) {
            tonemapping = true;
        }
        ImGui::EndMenu();
    }

    void Graphics::Editor() {
        if (general) {
            ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Appearing);
            ImGui::Begin("[GRAPHICS] General", &general);
            static bool wireFrame = true;
            ImGui::Checkbox("Wireframe", &wireFrame);
            static bool lods = false;
            ImGui::Checkbox("Use LODs", &lods);
            ImGui::End();

            this->SetPolygonMode(!wireFrame ? GraphicsPolygonMode::FILL
                                                            : GraphicsPolygonMode::LINE);
            this->UseLODs(lods);
        }
        
        if (clouds) {
            CloudConfig config = this->GetCloudConfig();

            ImGui::SetNextWindowSize(ImVec2(450, 400), ImGuiCond_Appearing);
            ImGui::Begin("[GRAPHICS] Clouds", &clouds);

            ImGui::Checkbox("Enabled", &config.enabled);

            ImGui::DragFloat3("Offset", &config.offset.x);
            ImGui::SliderFloat("Scale", &config.scale, 0.1f, 1.0f);
            ImGui::SliderFloat("Density Threshold", &config.densityThreshold, 0.0f, 0.5f);
            ImGui::SliderFloat("Density Multiplier", &config.densityMultiplier, 0.0f, 3.0f);

            ImGui::SliderFloat("Forward Scattering", &config.forwardScattering, 0.0f, 1.0f);
            ImGui::SliderFloat("Backward Scattering", &config.backwardScattering, 0.0f, 1.0f);
            ImGui::SliderFloat("Base Brightness", &config.baseBrightness, 0.0f, 5.0f);
            ImGui::SliderFloat("Phase Factor", &config.phaseFactor, 0.0f, 2.0f);

            ImGui::SliderFloat("Direct Light Absorbtion", &config.lightAbsorbtion, 0.0f, 3.0f);
            ImGui::SliderFloat("Inirect Light Absorbtion", &config.indirectLightAbsorbtion, 0.0f,
                               3.0f);
            ImGui::SliderFloat("Darkness Threshold", &config.darknessThreshold, 0.0f, 0.4f);

            if (ImGui::Button("Reset")) {
                config = CloudConfig{};
            }

            ImGui::End();

            this->SetCloudConfig(config);
        }
    }
}  // namespace UniverseEngine
#endif