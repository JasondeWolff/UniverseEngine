#include "MathUtil.h"

#include <algorithm>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <random>
#include <sstream>

namespace UniverseEngine {
    // Source:
    // https://stackoverflow.com/questions/24365331/how-can-i-generate-uuid-in-c-without-using-boost-library
    std::string GenerateUUID() {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        static std::uniform_int_distribution<uint64_t> dis(0, 15);
        static std::uniform_int_distribution<uint64_t> dis2(8, 11);

        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
    }

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

    uint32_t DivideUp(uint32_t n, uint32_t d) {
        return (std::max(2u, n) - 1) / d + 1;
    }
}  // namespace UniverseEngine