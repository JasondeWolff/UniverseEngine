#include "Camera.h"

namespace UniverseEngine {
    Camera::Camera(float fov, float aspect, float near, float far)
        : transform{}, fov(fov), aspect(aspect), near(near), far(far), matrix{}, dirty(true) {
        transform.SetTranslation(glm::vec3(0.0, 0.0, 10.0));
    }

    float Camera::GetFov() const {
        return this->fov;
    }

    float Camera::GetNear() const {
        return this->near;
    }

    float Camera::GetFar() const {
        return this->far;
    }

    float Camera::GetAspect() const {
        return this->aspect;
    }

    const glm::mat4& Camera::GetMatrix() const {
        if (this->dirty) {
            this->dirty = false;
            this->UpdateMatrix();
        }

        return this->matrix;
    }

    void Camera::SetFov(float fov) {
        this->fov = fov;
        void SetNear(float near);
    }

    void Camera::SetNear(float near) {
        this->near = near;
        void SetNear(float near);
    }

    void Camera::SetFar(float far) {
        this->far = far;
        this->dirty = true;
    }

    void Camera::SetAspect(float aspect) {
        this->aspect = aspect;
        this->dirty = true;
    }

    void Camera::UpdateMatrix() const {
        this->matrix = glm::perspective(glm::radians(this->fov), this->aspect, this->near, this->far);
    }
}  // namespace UniverseEngine