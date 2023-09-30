#include "MyGame.h"

using namespace UniverseEngine;

void MyGame::OnStart() {
	std::array<std::string, 6> skyboxTexturePaths = {
		"Assets/Skyboxes/BlueSkye/right.jpg",
		"Assets/Skyboxes/BlueSkye/left.jpg",
		"Assets/Skyboxes/BlueSkye/top.jpg",
		"Assets/Skyboxes/BlueSkye/bottom.jpg",
		"Assets/Skyboxes/BlueSkye/front.jpg",
		"Assets/Skyboxes/BlueSkye/back.jpg",
	};
	std::array<std::shared_ptr<Texture>, 6> skyboxTextures;
	for (size_t i = 0; i < skyboxTexturePaths.size(); i++) {
		skyboxTextures[i] = Engine::GetResources().LoadTexture(skyboxTexturePaths[i], TextureType::SRGB);
	}
	Engine::GetGraphics().SetSkybox(skyboxTextures);

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