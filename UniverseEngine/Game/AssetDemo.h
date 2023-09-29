#pragma once

#include <UniverseEngine.h>

#include "FreeFormCamera.h"

class AssetDemo : public UniverseEngine::Game {
public:
	void OnStart() override;
	void Update(float deltaTime) override;
	void OnClose() override;

private:
	FreeFormCamera camera;

	std::vector<std::shared_ptr<UniverseEngine::Scene>> scenes;
	std::vector<std::shared_ptr<UniverseEngine::SceneInstance>> sceneInstances;

	std::shared_ptr<UniverseEngine::Scene> sponza;
};