#include <array>
#include "TerrianDemo.h"

using namespace UniverseEngine;

void TerrianDemo::OnStart() {
	Resources& resources = Engine::GetResources();
	World& world = Engine::GetWorld();
	Graphics& graphics = Engine::GetGraphics();

	graphics.UseLODs(false);
	graphics.SetPolygonMode(GraphicsPolygonMode::LINE);

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

	auto GeneratedTerrianInstance = Engine::GetWorld().GenerateWorld();

	this->camera.Start();
}

void TerrianDemo::Update(float deltaTime) {
	if (Engine::GetInput().GetKey(KeyCode::ESCAPE)) {
		Engine::Quit();
	}

	this->camera.Update(deltaTime);

	bool uiActive = true;
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Always);
	ImGui::Begin("LODDemo", &uiActive);
	static bool wireFrame = false;
	ImGui::Checkbox("Wireframe", &wireFrame);
	ImGui::End();

	Engine::GetGraphics().SetPolygonMode(!wireFrame ? GraphicsPolygonMode::FILL : GraphicsPolygonMode::LINE);
}

void TerrianDemo::OnClose() {

}