#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
	Resources& resources = Engine::GetResources();
	World& world = Engine::GetWorld();

	this->hSusan = resources.LoadScene("Assets/susanMultiple.obj");
	this->hSusanInstance = world.AddSceneInstance(this->hSusan);

	this->hCube = resources.LoadScene("Assets/cube.gltf");
	this->hCubeInstance = world.AddSceneInstance(this->hCube);

	Scene& scene = resources.GetScene(hCube).Value();
	scene.worldPosition.resize(scene.meshes.size());
	scene.worldPosition.at(0) = glm::vec3(5, 0, 0);


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