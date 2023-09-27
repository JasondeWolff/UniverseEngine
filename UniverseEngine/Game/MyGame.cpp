#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
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