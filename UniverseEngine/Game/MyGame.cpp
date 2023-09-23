#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
	Resources& resources = Engine::GetResources();
	World& world = Engine::GetWorld();

	/*this->hSusan = resources.LoadScene("Assets/susanMultiple.obj");
	this->hSusanInstance = world.AddSceneInstance(this->hSusan);

	this->hCube = resources.LoadScene("Assets/cube.gltf");
	this->hCubeInstance = world.AddSceneInstance(this->hCube);*/

	this->hSpot = resources.LoadScene("Assets/spot.obj");
	this->hSpotInstance = world.AddSceneInstance(this->hSpot);

	TerrianGenerator customTerrian;
	customTerrian.GenerateRandomChunk();

	this->camera.Start();
}

void MyGame::Update(float deltaTime) {
	if (Engine::GetInput().GetKey(KeyCode::ESCAPE)) {
		Engine::Quit();
	}

	this->camera.Update(deltaTime);
}

void MyGame::OnClose() {

}