#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
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
		skyboxTextures[i] = Engine::GetResources().LoadTexture(skyboxTexturePaths[i], TextureType::SRGB);
	}
	Engine::GetGraphics().SetSkybox(skyboxTextures);

	Engine::GetGraphics().SetPolygonMode(GraphicsPolygonMode::LINE);

	auto GeneratedTerrianInstance = Engine::GetWorld().GenerateWorld();

	this->freeFormCamera.Start();
	this->spaceShip.Start();
}

void MyGame::Update(float deltaTime) {
	if (Engine::GetInput().GetKey(KeyCode::ESCAPE)) {
		Engine::Quit();
	}

	this->freeFormCamera.Update(deltaTime);
	this->spaceShip.Update(deltaTime);

	bool my_tool_active = true;
	ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Generate samples and plot them
	float samples[100];
	for (int n = 0; n < 100; n++)
		samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
	ImGui::PlotLines("Samples", samples, 100);

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();
}

void MyGame::OnClose() {

}