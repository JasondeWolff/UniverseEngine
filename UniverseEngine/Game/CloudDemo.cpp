#include "CloudDemo.h"

#include <array>

using namespace UniverseEngine;

void CloudDemo::OnStart() {
	Resources& resources = Engine::GetResources();
	World& world = Engine::GetWorld();
	Graphics& graphics = Engine::GetGraphics();

	std::array<std::string, 6> skyboxTexturePaths = {
		"Assets/Skyboxes/BlueSkye/right.jpg",
		"Assets/Skyboxes/BlueSkye/left.jpg",
		"Assets/Skyboxes/BlueSkye/top.jpg",
		"Assets/Skyboxes/BlueSkye/bottom.jpg",
		"Assets/Skyboxes/BlueSkye/front.jpg",
		"Assets/Skyboxes/BlueSkye/back.jpg",
	};
	std::array<std::shared_ptr<Texture>, 6> skyboxTextures;
	for (size_t i = 0; i < skyboxTexturePaths.size(); i++) {
		skyboxTextures[i] = resources.LoadTexture(skyboxTexturePaths[i], TextureType::SRGB);
	}
	graphics.SetSkybox(skyboxTextures);

	this->camera.Start();
}

void CloudDemo::Update(float deltaTime) {
	if (Engine::GetInput().GetKey(KeyCode::ESCAPE)) {
		Engine::Quit();
	}

	this->camera.Update(deltaTime);

	CloudConfig config = Engine::GetGraphics().GetCloudConfig();
	config.enabled = true;

	bool uiActive = true;
	ImGui::SetNextWindowSize(ImVec2(450, 400), ImGuiCond_Always);
	ImGui::Begin("CloudDemo", &uiActive);
	ImGui::ColorEdit3("Sun Color", &Engine::GetWorld().sun.lightSource.color.x);
	
	ImGui::DragFloat3("Offset", &config.offset.x);
	ImGui::SliderFloat("Scale", &config.scale, 0.1f, 1.0f);
	ImGui::SliderFloat("Density Threshold", &config.densityThreshold, 0.0f, 0.5f);
	ImGui::SliderFloat("Density Multiplier", &config.densityMultiplier, 0.0f, 3.0f);
	
	ImGui::SliderFloat("Forward Scattering", &config.forwardScattering, 0.0f, 1.0f);
	ImGui::SliderFloat("Backward Scattering", &config.backwardScattering, 0.0f, 1.0f);
	ImGui::SliderFloat("Base Brightness", &config.baseBrightness, 0.0f, 5.0f);
	ImGui::SliderFloat("Phase Factor", &config.phaseFactor, 0.0f, 2.0f);

	ImGui::SliderFloat("Direct Light Absorbtion", &config.lightAbsorbtion, 0.0f, 3.0f);
	ImGui::SliderFloat("Inirect Light Absorbtion", &config.indirectLightAbsorbtion, 0.0f, 3.0f);
	ImGui::SliderFloat("Darkness Threshold", &config.darknessThreshold, 0.0f, 0.4f);

	if (ImGui::Button("Reset")) {
		config = CloudConfig{};
	}

	ImGui::End();

	Engine::GetGraphics().SetCloudConfig(config);
}

void CloudDemo::OnClose() {

}