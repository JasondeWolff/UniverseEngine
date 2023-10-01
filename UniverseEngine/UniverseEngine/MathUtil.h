#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <tinyusdz/value-types.hh>

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

    glm::quat EulerToQuat(const glm::vec3& euler);
    glm::vec3 QuatToEuler(const glm::quat& rotation);

    glm::vec3 ClampMagnitude(const glm::vec3& v, float magnitude = 1.0f);
    glm::vec2 ClampMagnitude(const glm::vec2& v, float magnitude = 1.0f);

    glm::vec3 ToGlm(const tinyusdz::value::float3 f3);
    glm::vec2 ToGlm(const tinyusdz::value::float2 f2);
}  // namespace UniverseEngine