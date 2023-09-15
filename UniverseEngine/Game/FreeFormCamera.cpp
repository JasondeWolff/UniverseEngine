#include "FreeFormCamera.h"

#include <UniverseEngine.h>
using namespace UniverseEngine;

FreeFormCamera::FreeFormCamera() : movementSpeed(5.0), lookSensitivity(0.3) {}

void FreeFormCamera::Start() {
	Engine::GetInput().SetCursorMode(CursorMode::DISABLED);
}

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
	if (input.GetKey(KeyCode::E)) {
		translation += camera.transform.GetUp();
	}
	if (input.GetKey(KeyCode::Q)) {
		translation -= camera.transform.GetUp();
	}
	translation *= deltaTime * this->movementSpeed;

	this->rotationEuler.x -= input.GetMouseDelta().y * this->lookSensitivity;
	this->rotationEuler.z -= input.GetMouseDelta().x * this->lookSensitivity;

	camera.transform.Translate(translation);
	camera.transform.SetRotation(EulerToQuat(rotationEuler));
}