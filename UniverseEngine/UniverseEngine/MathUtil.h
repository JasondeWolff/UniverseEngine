#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace UniverseEngine {
	inline constexpr glm::vec3 Right() {
		return glm::vec3(1.0, 0.0, 0.0);
	}

	inline constexpr glm::vec3 Up() {
		return glm::vec3(0.0, 1.0, 0.0);
	}

	inline constexpr glm::vec3 Forward() {
		return glm::vec3(0.0, 0.0, 1.0);
	}

	glm::quat EulerToQuat(const glm::vec3& euler);
	glm::vec3 QuatToEuler(const glm::quat& rotation);
}