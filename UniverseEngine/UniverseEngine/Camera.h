#pragma once

#include "Transform.h"

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

namespace UniverseEngine {
    class Camera {
    public:
        Camera(float fov = 70.0, float aspect = 1.0, float near = 0.1, float far = 5000.0);

        Transform transform;

        float GetFov() const;
        float GetNear() const;
        float GetFar() const;
        float GetAspect() const;
        const glm::mat4& GetMatrix() const;

        void SetFov(float fov);
        void SetNear(float near);
        void SetFar(float far);
        void SetAspect(float aspect);

    private:
        float fov;
        float near;
        float far;
        float aspect;

        mutable glm::mat4 matrix;
        mutable bool dirty;

        void UpdateMatrix() const;
    };
}  // namespace UniverseEngine