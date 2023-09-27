#include "SpaceShip.h"

using namespace UniverseEngine;

void SpaceShip::Start() {
	Engine::GetInput().SetCursorMode(CursorMode::DISABLED);

	this->scene = Engine::GetResources().LoadScene("Assets/Models/SpaceShip/SpaceShip.gltf");
	this->sceneInstance = Engine::GetWorld().AddSceneInstance(this->scene);

	auto& camera = Engine::GetWorld().camera;
	camera.transform.Rotate(EulerToQuat(Up() * 180.0f));
	camera.SetFov(75.0f);
}

void SpaceShip::Update(float deltaTime) {
	auto& input = Engine::GetInput();
	auto& gamepad = input.GetGamepad();
	auto& camera = Engine::GetWorld().camera;

	auto& cameraTransform = camera.transform;
	auto& shipTransform = this->sceneInstance->transform;

	cameraTransform.SetTranslation(glm::mix(cameraTransform.GetTranslation(), shipTransform.GetTranslation() + this->cameraOffset, 1.0f / this->cameraSmoothing * deltaTime));
}