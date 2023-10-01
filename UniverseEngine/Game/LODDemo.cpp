#include "LODDemo.h"

#include <array>

using namespace UniverseEngine;

void LODDemo::OnStart() {
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

	std::vector<std::pair<std::string, Transform>> scenePaths = {
		{"Assets/Models/Showcase/Dragon/Dragon.gltf", Transform(glm::vec3{}, EulerToQuat(Right() * 90.0f), glm::vec3(0.2f))},
	};

	glm::vec3 translation{};
	for (auto scenePath : scenePaths) {
		auto scene = resources.LoadScene(scenePath.first);
		auto sceneInstance = world.AddSceneInstance(scene);

		sceneInstance->transform.SetMatrix(scenePath.second.GetMatrix());
		sceneInstance->transform.Translate(translation);
		translation += Right() * 3.0f;

		this->scenes.push_back(scene);
		this->sceneInstances.push_back(sceneInstance);
	}

	this->camera.Start();
}

void LODDemo::Update(float deltaTime) {
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

void LODDemo::OnClose() {

}