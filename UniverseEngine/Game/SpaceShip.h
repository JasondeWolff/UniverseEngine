#pragma once

#include <UniverseEngine.h>

#include <glm/vec3.hpp>

class SpaceShip {
public:
	SpaceShip() = default;

	void Start();
	void Update(float deltaTime);

	float cameraSmoothing = 0.2f;
	glm::vec3 cameraOffset = glm::vec3(0.0f, 4.0f, -15.0f);

private:
	std::shared_ptr<UniverseEngine::Scene> scene;
	std::shared_ptr<UniverseEngine::SceneInstance> sceneInstance;

	glm::vec3 cameraTargetPosition;
};