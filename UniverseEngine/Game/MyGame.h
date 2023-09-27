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

	std::shared_ptr<UniverseEngine::Scene> susan;
	std::shared_ptr<UniverseEngine::Scene> spot;
	std::shared_ptr<UniverseEngine::Scene> buggy;

	std::shared_ptr<UniverseEngine::SceneInstance> susanInstance;
	std::shared_ptr<UniverseEngine::SceneInstance> spotInstance;
	std::shared_ptr<UniverseEngine::SceneInstance> buggyInstance;
};