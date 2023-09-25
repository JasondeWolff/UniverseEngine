#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
	Resources& resources = Engine::GetResources();
	World& world = Engine::GetWorld();

	/*this->hCube = resources.LoadScene("Assets/cube.gltf");
	this->hCubeInstance = world.AddSceneInstance(this->hCube);*/

	this->hSpot = resources.LoadScene("Assets/spot.obj");
	this->hSpotInstance = world.AddSceneInstance(this->hSpot);

	this->hSusan = resources.LoadScene("Assets/susanMultiple.obj");
	this->hSusanInstance = world.AddSceneInstance(this->hSusan);

	TerrianGenerator customTerrian;
	customTerrian.GenerateRandomChunk();

	this->camera.Start();
}

void MyGame::Update(float deltaTime) {
	if (Engine::GetInput().GetKey(KeyCode::ESCAPE)) {
		Engine::Quit();
	}

	this->camera.Update(deltaTime);

	this->hSusanInstance->transform.Rotate(EulerToQuat(Up() * deltaTime * 15.0f));
}

void MyGame::OnClose() {

}