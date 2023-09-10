#include "Transform.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace UniverseEngine {
    Transform::Transform()
        : matrixDirty(true) {
        this->SetMatrix(glm::identity<glm::mat4>());
    }

    Transform::Transform(const glm::mat4& matrix)
        : matrixDirty(true) {
        this->SetMatrix(matrix);
    }

    Transform::Transform(const glm::vec3& translation,
                         const glm::quat& rotation, const glm::vec3& scale)
        : translation(translation),
          rotation(rotation),
          scale(scale),
          matrix{},
          matrixDirty(true) {
    }

    glm::vec3 Transform::GetRight() const {
        return this->rotation * Right();
    }

    glm::vec3 Transform::GetUp() const {
        return this->rotation * Up();
    }

    glm::vec3 Transform::GetForward() const {
        return this->rotation * Forward();
    }

    const glm::vec3& Transform::GetTranslation() const {
        return this->translation;
    }

    const glm::quat& Transform::GetRotation() const {
        return this->rotation;
    }

    const glm::vec3& Transform::GetScale() const {
        return this->scale;
    }

    const glm::mat4& Transform::GetMatrix() const {
        if (this->matrixDirty) {
            UpdateMatrix();
        }
        return this->matrix;
    }

    void Transform::SetTranslation(const glm::vec3& translation) {
        this->translation = translation;
        this->matrixDirty = true;
    }

    void Transform::SetRotation(const glm::quat& rotation) {
        this->rotation = rotation;
        this->matrixDirty = true;
    }

    void Transform::SetScale(const glm::vec3& scale) {
        this->scale = scale;
        this->matrixDirty = true;
    }

    void Transform::SetMatrix(const glm::mat4& matrix) {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(matrix, this->scale, this->rotation, this->translation,
                       skew, perspective);

        this->matrix = matrix;
        this->matrixDirty = false;
    }

    void Transform::Translate(const glm::vec3& translate) {
        this->translation += translate;
        this->matrixDirty = true;
    }

    void Transform::Rotate(const glm::quat& rotation) {
        this->rotation *= rotation;
        this->matrixDirty = true;
    }

    void Transform::Scale(const glm::vec3& scale) {
        this->scale *= scale;
        this->matrixDirty = true;
    }

    void Transform::UpdateMatrix() const {
        this->matrix = glm::scale(this->matrix, this->scale);
        this->matrix *= glm::toMat4(this->rotation);
        this->matrix = glm::translate(this->matrix, this->translation);

        this->matrixDirty = false;
    }
}  // namespace UniverseEngine