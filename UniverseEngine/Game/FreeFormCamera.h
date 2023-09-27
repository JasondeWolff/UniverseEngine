#pragma once

#include <glm/vec3.hpp>

class FreeFormCamera {
public:
	FreeFormCamera() = default;

	void Start();
	void Update(float deltaTime);

	float movementSpeed = 5.0f;
	float lookSensitivity = 0.3f;

private:
	glm::vec3 rotationEuler = glm::vec3{};
	bool active = false;
};