#pragma once

#include <UniverseEngine.h>

#include "FreeFormCamera.h"

class TerrianDemo : public UniverseEngine::Game {
public:
	void OnStart() override;
	void Update(float deltaTime) override;
	void OnClose() override;

private:
	FreeFormCamera camera;

};