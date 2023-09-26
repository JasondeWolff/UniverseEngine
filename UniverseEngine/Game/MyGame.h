#pragma once

#include <UniverseEngine.h>

#include "FreeFormCamera.h"

class MyGame : public UniverseEngine::Game {
public:
	void OnStart() override;
	void Update(float deltaTime) override;
	void OnClose() override;

private:
	FreeFormCamera camera;

	std::shared_ptr<UniverseEngine::Scene> hSusan;
	std::shared_ptr<UniverseEngine::Scene> hCube;
	std::shared_ptr<UniverseEngine::Scene> hSpot;
	std::shared_ptr<UniverseEngine::Scene> hBuggy;

	std::shared_ptr<UniverseEngine::SceneInstance> hSusanInstance;
	std::shared_ptr<UniverseEngine::SceneInstance> hCubeInstance;
	std::shared_ptr<UniverseEngine::SceneInstance> hSpotInstance;
	std::shared_ptr<UniverseEngine::SceneInstance> hBuggyInstance;
};