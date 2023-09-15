#include "FreeFormCamera.h"

#include <UniverseEngine.h>
using namespace UniverseEngine;

FreeFormCamera::FreeFormCamera() : movementSpeed(5.0), lookSensitivity(1.0) {}

void FreeFormCamera::Update(float deltaTime) {
	auto& input = Engine::GetInput();
	auto& camera = Engine::GetWorld().camera;

	glm::vec3 translation{};
	if (input.GetKey(KeyCode::W)) {
		translation += camera.transform.GetForward();
	}
	if (input.GetKey(KeyCode::S)) {
		translation -= camera.transform.GetForward();
	}
	if (input.GetKey(KeyCode::D)) {
		translation += camera.transform.GetRight();
	}
	if (input.GetKey(KeyCode::A)) {
		translation -= camera.transform.GetRight();
	}
	translation *= deltaTime * this->movementSpeed;

	camera.transform.Translate(translation);
}