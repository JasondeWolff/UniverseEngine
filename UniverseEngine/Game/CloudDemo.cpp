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

	bool uiActive = true;
	ImGui::SetNextWindowSize(ImVec2(350, 250), ImGuiCond_Always);
	ImGui::Begin("CloudDemo", &uiActive);
	ImGui::DragFloat3("Offset", &config.offset.x);
	ImGui::SliderFloat("Scale", &config.scale, 0.1f, 1.0f);
	ImGui::SliderFloat("Density Threshold", &config.densityThreshold, 0.0f, 0.5f);
	ImGui::SliderFloat("Density Multiplier", &config.densityMultiplier, 0.0f, 3.0f);
	ImGui::ColorEdit3("Sun Color", &Engine::GetWorld().sun.lightSource.color.x);
	ImGui::End();

	Engine::GetGraphics().SetCloudConfig(config);
}

void CloudDemo::OnClose() {

}