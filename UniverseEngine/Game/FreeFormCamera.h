#pragma once

#include <glm/vec3.hpp>

class FreeFormCamera {
public:
	FreeFormCamera();

	void Start();
	void Update(float deltaTime);

	float movementSpeed;
	float lookSensitivity;

private:
	glm::vec3 rotationEuler;
};