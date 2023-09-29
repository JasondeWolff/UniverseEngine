#include "AssetDemo.h"

using namespace UniverseEngine;

void AssetDemo::OnStart() {
	Resources& resources = Engine::GetResources();
	World& world = Engine::GetWorld();

	std::vector<std::pair<std::string, Transform>> scenePaths = {
		{"Assets/Models/Showcase/AntiqueCamera/AntiqueCamera.gltf", Transform(glm::vec3{}, EulerToQuat(Right() * 180.0f), glm::vec3(0.3f))},
		{"Assets/Models/Showcase/Corset/Corset.gltf", Transform(glm::vec3{}, EulerToQuat(Forward() * 180.0f), glm::vec3(30.0f))},
		{"Assets/Models/Showcase/DamagedHelmet/DamagedHelmet.gltf", Transform(glm::vec3(0.0f, 1.0f, 0.0f), EulerToQuat(Up() * 90.0f + Right() * 90.0f), glm::vec3(1.0f))},
		{"Assets/Models/Showcase/FlightHelmet/FlightHelmet.gltf", Transform(glm::vec3{}, EulerToQuat(glm::vec3{}), glm::vec3(2.5f))},
		{"Assets/Models/Showcase/SciFiHelmet/SciFiHelmet.gltf", Transform(glm::vec3(0.0f, 0.7f, 0.0f), EulerToQuat(glm::vec3{}), glm::vec3(0.7f))},
		{"Assets/Models/Showcase/Buggy/Buggy.gltf", Transform(glm::vec3(0.0f, 0.1f, 0.0f), EulerToQuat(glm::vec3{}), glm::vec3(0.02f))},
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

	this->sponza = resources.LoadScene("Assets/Models/Showcase/Sponza/Sponza.gltf");
	world.AddSceneInstance(this->sponza)->transform.SetScale(glm::vec3(2.0));

	this->camera.Start();
}

void AssetDemo::Update(float deltaTime) {
	if (Engine::GetInput().GetKey(KeyCode::ESCAPE)) {
		Engine::Quit();
	}

	this->camera.Update(deltaTime);
}

void AssetDemo::OnClose() {

}