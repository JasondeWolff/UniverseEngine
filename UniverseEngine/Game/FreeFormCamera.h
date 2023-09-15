#pragma once

class FreeFormCamera {
public:
	FreeFormCamera();

	void Update(float deltaTime);

	float movementSpeed;
	float lookSensitivity;
};