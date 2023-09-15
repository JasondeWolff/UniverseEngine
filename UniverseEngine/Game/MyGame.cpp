#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
	this->hSusan = Engine::GetResources().LoadScene("Assets/susanMultiple.obj");
	this->hCube = Engine::GetResources().LoadScene("Assets/cube.usd");

	this->hSusanInstance = Engine::GetWorld().AddSceneInstance(this->hSusan);
	this->hCubeInstance = Engine::GetWorld().AddSceneInstance(this->hCube);
}

void MyGame::Update(float deltaTime) {

}

void MyGame::OnClose() {

}