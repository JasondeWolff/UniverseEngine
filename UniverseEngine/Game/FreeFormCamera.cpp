#include "FreeFormCamera.h"

#include <UniverseEngine.h>
using namespace UniverseEngine;

FreeFormCamera::FreeFormCamera() : movementSpeed(5.0f), lookSensitivity(0.3f) {}

void FreeFormCamera::Start() {
	Engine::GetInput().SetCursorMode(CursorMode::DISABLED);
}

void FreeFormCamera::Update(float deltaTime) {
	auto& input = Engine::GetInput();
	auto& gamepad = input.GetGamepad();
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
	if (input.GetKey(KeyCode::E) || gamepad.GetButton(GamepadButton::B)) {
		translation += camera.transform.GetUp();
	}
	if (input.GetKey(KeyCode::Q) || gamepad.GetButton(GamepadButton::A)) {
		translation -= camera.transform.GetUp();
	}

	this->rotationEuler.x -= input.GetMouseDelta().y * this->lookSensitivity;
	this->rotationEuler.z -= input.GetMouseDelta().x * this->lookSensitivity;

	this->rotationEuler.x -= gamepad.GetAxis(GamepadAxis::RIGHT_Y) * this->lookSensitivity * 4.0f * deltaTime * 60.0;
	this->rotationEuler.z -= gamepad.GetAxis(GamepadAxis::RIGHT_X) * this->lookSensitivity * 4.0f * deltaTime * 60.0;
	translation += gamepad.GetAxis(GamepadAxis::LEFT_X) * camera.transform.GetRight() + gamepad.GetAxis(GamepadAxis::LEFT_Y) * -camera.transform.GetForward();

	if (glm::dot(translation, translation) > 0.0) {
		translation = glm::normalize(translation) * deltaTime * this->movementSpeed;
	}

	camera.transform.Translate(translation);
	camera.transform.SetRotation(EulerToQuat(rotationEuler));
}