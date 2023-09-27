#include "SpaceShip.h"

using namespace UniverseEngine;

void SpaceShip::Start() {
	Engine::GetInput().SetCursorMode(CursorMode::ENABLED);

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

	glm::vec3 cameraTargetPosition = shipTransform.GetTranslation() + shipTransform.GetRotation() * this->cameraOffset;
	cameraTransform.SetTranslation(glm::mix(cameraTransform.GetTranslation(), cameraTargetPosition, 1.0f / this->cameraSmoothing * deltaTime));
	cameraTransform.SetRotation(glm::quatLookAt(glm::normalize(shipTransform.GetTranslation() - cameraTransform.GetTranslation()), Up()));

	if (input.GetKey(KeyCode::W)) {
		this->throttle += this->config.maxDeltaThrottle * deltaTime;
	} else if (input.GetKey(KeyCode::S)) {
		this->throttle -= this->config.maxDeltaThrottle * deltaTime;
	}
	this->throttle = glm::clamp(this->throttle, -this->config.maxThrottle, this->config.maxThrottle);

	this->speed += this->throttle * deltaTime;
	this->speed = glm::clamp(this->speed, -this->config.maxReverseSpeed, this->config.maxSpeed);

	UE_INFO("----SHIP----");
	UE_INFO("Speed:     %f", this->speed);
	UE_INFO("Throttle:  %f", this->throttle);

	shipTransform.Translate(-shipTransform.GetForward() * this->speed * deltaTime);

	auto& window = Engine::GetGraphics().GetWindow();
	float horizontalInput = input.GetMousePosition().x - (static_cast<float>(window.Width()) * 0.5f);
	float verticalInput = input.GetMousePosition().y - (static_cast<float>(window.Height()) * 0.5f);

	float maxDragDistance = static_cast<float>(window.Height()) * 0.25f;
	glm::vec2 inputVector(-horizontalInput / maxDragDistance, verticalInput / maxDragDistance);
	inputVector = ClampMagnitude(inputVector, 1.0f);

	UE_INFO("Steering: %f %f", inputVector.x, inputVector.y);

	this->desiredEulerRotation.y += inputVector.x * this->config.maneuverability * deltaTime;
	this->desiredEulerRotation.x += inputVector.y * this->config.maneuverability * deltaTime;
	shipTransform.SetRotation(glm::mix(shipTransform.GetRotation(), EulerToQuat(desiredEulerRotation), deltaTime));
}