#include "MathUtil.h"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

namespace UniverseEngine {
    glm::quat EulerToQuat(const glm::vec3& euler) {
        return glm::quat(glm::radians(euler));
    }

    glm::vec3 QuatToEuler(const glm::quat& rotation) {
        return glm::degrees(glm::eulerAngles(rotation));
    }

    glm::vec3 ClampMagnitude(const glm::vec3& v, float magnitude) {
        return v * glm::min(glm::length(v), magnitude);
    }

    glm::vec2 ClampMagnitude(const glm::vec2& v, float magnitude) {
        if (glm::length(v) == 0.0f)
            return v;
        return glm::normalize(v) * glm::min(glm::length(v), magnitude);
    }

    glm::vec3 ToGlm(const tinyusdz::value::float3 f3) {
        const float* data = f3.data();
        return glm::vec3(*data, *(data + 1), *(data + 2));
    }

    glm::vec2 ToGlm(const tinyusdz::value::float2 f2) {
        const float* data = f2.data();
        return glm::vec2(*data, *(data + 1));
    }
}  // namespace UniverseEngine