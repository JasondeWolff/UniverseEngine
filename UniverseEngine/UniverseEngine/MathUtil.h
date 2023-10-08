#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <string>

namespace UniverseEngine {
    inline constexpr glm::vec3 Right() {
        return glm::vec3(1.0, 0.0, 0.0);
    }
    inline constexpr glm::vec3 Up() {
        return glm::vec3(0.0, 1.0, 0.0);
    }
    inline constexpr glm::vec3 Forward() {
        return glm::vec3(0.0, 0.0, -1.0);
    }

    std::string GenerateUUID();

    glm::quat EulerToQuat(const glm::vec3& euler);
    glm::vec3 QuatToEuler(const glm::quat& rotation);

    glm::vec3 ClampMagnitude(const glm::vec3& v, float magnitude = 1.0f);
    glm::vec2 ClampMagnitude(const glm::vec2& v, float magnitude = 1.0f);

    uint32_t DivideUp(uint32_t n, uint32_t d);
}  // namespace UniverseEngine