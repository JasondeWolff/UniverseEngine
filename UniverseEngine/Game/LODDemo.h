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

	std::shared_ptr<UniverseEngine::Scene> bunny;
	std::shared_ptr<UniverseEngine::SceneInstance> bunnyInstance;

	std::vector<std::shared_ptr<UniverseEngine::Scene>> simplifiedBunnies;
	std::vector<std::shared_ptr<UniverseEngine::SceneInstance>> simplifiedBunnyInstances;
};