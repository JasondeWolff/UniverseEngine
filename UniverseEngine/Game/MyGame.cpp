#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
	Resources& resources = Engine::GetResources();
	World& world = Engine::GetWorld();

	this->spot = resources.LoadScene("Assets/Models/spot.obj");
	this->spotInstance = world.AddSceneInstance(this->spot);

	this->susan = resources.LoadScene("Assets/Models/susanMultiple.obj");
	this->susanInstance = world.AddSceneInstance(this->susan);

	this->buggy = resources.LoadScene("Assets/Models/Buggy.gltf");
	this->buggyInstance = world.AddSceneInstance(this->buggy);
	this->buggyInstance->transform.SetScale(glm::vec3(0.1, 0.1, 0.1));

	TerrianGenerator customTerrian;
	customTerrian.GenerateRandomChunk();

	this->camera.Start();
}

void MyGame::Update(float deltaTime) {
	if (Engine::GetInput().GetKey(KeyCode::ESCAPE)) {
		Engine::Quit();
	}

	this->camera.Update(deltaTime);

	this->susanInstance->transform.Rotate(EulerToQuat(Up() * deltaTime * 15.0f));
}

void MyGame::OnClose() {

}