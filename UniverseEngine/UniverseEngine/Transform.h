#pragma once

#include <glm/mat4x4.hpp>

#include "Math.h"

namespace UniverseEngine {
	class Transform {
	public:
		Transform(const glm::mat4& matrix);
		Transform(const glm::vec3& translation = glm::zero<glm::vec3>(), const glm::quat& rotation = glm::identity<glm::quat>(), const glm::vec3& scale = glm::one<glm::vec3>());

		const glm::vec3& GetTranslation() const;
		const glm::quat& GetRotation() const;
		const glm::vec3& GetScale() const;
		const glm::mat4& GetMatrix() const;

		void SetTranslation(const glm::vec3& translation);
		void SetRotation(const glm::quat& rotation);
		void SetScale(const glm::vec3& scale);
		void SetMatrix(const glm::mat4& matrix);

		void Translate(const glm::vec3& translate);
		void Rotate(const glm::quat& rotation);
		void Scale(const glm::vec3& scale);

	private:
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;

		mutable glm::mat4 matrix;
		mutable bool matrixDirty;

		void UpdateMatrix() const;
	};
}