#pragma once

#include <UniverseEngine.h>

#include <glm/vec3.hpp>

struct SpaceShipConfig {
	// Max speed in m/s
	float maxSpeed = 50.0f;
	// Max reverse speed in m/s
	float maxReverseSpeed = 15.0f;

	// Max in- or decrease in m/s
	float maxThrottle = 20.0f;
	// Max throttle chance per second from user
	float maxDeltaThrottle = 10.0f;

	float maneuverability = 30.0f;
};

class SpaceShip {
public:
	SpaceShip() = default;

	void Start();
	void Update(float deltaTime);

	float cameraSmoothing = 0.2f;
	glm::vec3 cameraOffset = glm::vec3(0.0f, 4.0f, -15.0f);

	SpaceShipConfig config;

private:
	std::shared_ptr<UniverseEngine::Scene> scene;
	std::shared_ptr<UniverseEngine::SceneInstance> sceneInstance;

	float speed = 0.0f;
	float throttle = 0.0f;
	glm::vec3 desiredEulerRotation;
};