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

	std::vector<float> LODs = { 0.8f, 0.6f, 0.4f, 0.2f };

	this->bunny = resources.LoadScene("Assets/Models/Showcase/Bunny/Bunny.gltf");
	this->bunnyInstance = world.AddSceneInstance(this->bunny);

	UE_INFO("Bunny Tris LOD0: %i", this->bunny->meshes[0].indices.size() / 3);

	for (size_t i = 0; i < LODs.size(); i++) {
		auto simplifiedBunny = resources.CreateScene(this->bunny->meshes[0].BuildSimplified(LODs[i]));
		auto simplifiedBunnyInstance = world.AddSceneInstance(simplifiedBunny);
		simplifiedBunnyInstance->transform.SetMatrix(Transform(glm::vec3(i * 5.0f + 5.0f, 0.0, 0.0)).GetMatrix());

		this->simplifiedBunnies.push_back(simplifiedBunny);
		this->simplifiedBunnyInstances.push_back(simplifiedBunnyInstance);

		UE_INFO("Bunny Tris LOD%i: %i", (i + 1), simplifiedBunny->meshes[0].indices.size() / 3);
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