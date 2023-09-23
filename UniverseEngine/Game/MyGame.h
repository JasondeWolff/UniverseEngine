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

	UniverseEngine::Handle<UniverseEngine::Scene> hSusan;
	UniverseEngine::Handle<UniverseEngine::Scene> hCube;
	UniverseEngine::Handle<UniverseEngine::Scene> hSpot;

	UniverseEngine::Handle<UniverseEngine::SceneInstance> hSusanInstance;
	UniverseEngine::Handle<UniverseEngine::SceneInstance> hCubeInstance;
	UniverseEngine::Handle<UniverseEngine::SceneInstance> hSpotInstance;
};