#pragma once

#include <UniverseEngine.h>

#include "FreeFormCamera.h"

class LODDemo : public UniverseEngine::Game {
public:
	void OnStart() override;
	void Update(float deltaTime) override;
	void OnClose() override;

private:
	FreeFormCamera camera;

	std::shared_ptr<UniverseEngine::Scene> dragon;
	std::shared_ptr<UniverseEngine::SceneInstance> dragonInstance;

	std::shared_ptr<UniverseEngine::Scene> simplifiedDragon;
	std::shared_ptr<UniverseEngine::SceneInstance> simplifiedDragonInstance;
};