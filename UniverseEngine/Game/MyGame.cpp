#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
	this->terrainScene = Engine::GetResources().LoadScene("Assets/Models/MarsTerrain/MarsTerrain.gltf");
	this->terrainSceneInstance = Engine::GetWorld().AddSceneInstance(this->terrainScene);
	this->terrainSceneInstance->transform.Rotate(EulerToQuat(Right() * 90.0f));

	this->freeFormCamera.Start();
	this->spaceShip.Start();
}

void MyGame::Update(float deltaTime) {
	if (Engine::GetInput().GetKey(KeyCode::ESCAPE)) {
		Engine::Quit();
	}

	this->freeFormCamera.Update(deltaTime);
	this->spaceShip.Update(deltaTime);
}

void MyGame::OnClose() {

}