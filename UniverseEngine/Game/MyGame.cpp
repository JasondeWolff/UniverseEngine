#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
	this->hSusan = Engine::GetResources().LoadScene("Assets/susanMultiple.obj");
	this->hCube = Engine::GetResources().LoadScene("Assets/cube.usd");

	this->hSusanInstance = Engine::GetWorld().AddSceneInstance(this->hSusan);
	this->hCubeInstance = Engine::GetWorld().AddSceneInstance(this->hCube);

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