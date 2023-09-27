#pragma once

#include <UniverseEngine.h>

#include "SpaceShip.h"
#include "FreeFormCamera.h"

class MyGame : public UniverseEngine::Game {
public:
	void OnStart() override;
	void Update(float deltaTime) override;
	void OnClose() override;

private:
	FreeFormCamera freeFormCamera;
	SpaceShip spaceShip;

	std::shared_ptr<UniverseEngine::Scene> terrainScene;
	std::shared_ptr<UniverseEngine::SceneInstance> terrainSceneInstance;
};