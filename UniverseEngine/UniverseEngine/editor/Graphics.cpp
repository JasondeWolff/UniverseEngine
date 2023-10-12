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

            ImGui::DragFloat3("Weather Offset", &config.weatherOffset.x);
            ImGui::SliderFloat("Weather Scale", &config.weatherScale, 0.1f, 1.0f);
            ImGui::SliderFloat("Weather Density Threshold", &config.weatherDensityThreshold, 0.0f,
                               1.0f);
            ImGui::DragFloat3("Detail Offset", &config.detailOffset.x);
            ImGui::SliderFloat("Detail Scale", &config.detailScale, 0.1f, 1.0f);
            ImGui::SliderFloat("Detail Density Threshold", &config.detailDensityThreshold, 0.0f,
                               1.0f);
            ImGui::DragFloat3("Rough Offset", &config.roughOffset.x);
            ImGui::SliderFloat("Rough Scale", &config.roughScale, 0.1f, 1.0f);
            ImGui::SliderFloat("Rough Density Threshold", &config.roughDensityThreshold, 0.0f,
                               1.0f);

            ImGui::SliderFloat("Density Multiplier", &config.densityMultiplier, 0.0f, 3.0f);

            ImGui::SliderFloat("Forward Scattering", &config.forwardScattering, 0.0f, 1.0f);
            ImGui::SliderFloat("Backward Scattering", &config.backwardScattering, 0.0f, 1.0f);
            ImGui::SliderFloat("Base Brightness", &config.baseBrightness, 0.0f, 5.0f);
            ImGui::SliderFloat("Phase Factor", &config.phaseFactor, 0.0f, 2.0f);

            ImGui::SliderFloat("Direct Light Absorbtion", &config.lightAbsorbtion, 0.0f, 3.0f);
            ImGui::SliderFloat("Inirect Light Absorbtion", &config.indirectLightAbsorbtion, 0.0f,
                               3.0f);
            ImGui::SliderFloat("Darkness Threshold", &config.darknessThreshold, 0.0f, 0.4f);

            ImGui::Checkbox("SDF Debug", &config.sdfDebug);
            ImGui::SliderFloat("SDF Factor", &config.sdfFactor, 0.0f, 1.0f);

            if (ImGui::Button("Reset")) {
                config = CloudConfig{};
            }

            ImGui::End();

            this->SetCloudConfig(config);
        }
    }
}  // namespace UniverseEngine
#endif