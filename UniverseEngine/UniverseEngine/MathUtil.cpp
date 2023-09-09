#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Math.h"

namespace UniverseEngine {
    glm::quat EulerToQuat(const glm::vec3& euler) {
        return glm::toQuat(glm::orientate3(glm::radians(euler)));
    }

    glm::vec3 QuatToEuler(const glm::quat& rotation) {
        return glm::degrees(glm::eulerAngles(rotation));
    }
}  // namespace UniverseEngine