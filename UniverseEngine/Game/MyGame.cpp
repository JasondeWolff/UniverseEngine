#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
	this->hSusan = Engine::GetResources().LoadScene("Assets/susanMultiple.obj");
	this->hSusanInstance = Engine::GetWorld().AddSceneInstance(this->hSusan);

	this->hCube = Engine::GetResources().LoadScene("Assets/cube.gltf");
	auto hCubeInstance = Engine::GetWorld().AddSceneInstance(this->hCube);
	auto hCubeInstance2 = Engine::GetWorld().AddSceneInstance(this->hCube);
	{
		auto& cube = Engine::GetWorld().GetSceneInstance(hCubeInstance).Value();
		Transform trans;
		trans.SetTranslation(glm::vec3(10, 0, 0));
		cube.transform = trans;

		auto& cube2 = Engine::GetWorld().GetSceneInstance(hCubeInstance2).Value();
		Transform trans1;
		trans1.SetTranslation(glm::vec3(10, 1, 1));
		cube2.transform = trans1;
	}

	TerrianGenerator customTerrian;

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