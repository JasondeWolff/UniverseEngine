#pragma once

#include "Logging.h"

namespace UniverseEngine {
	template<typename T>
	struct OptionalPtr {
	public:
		static OptionalPtr Some(T* value) {
			return OptionalPtr(value);
		}

		static OptionalPtr None() {
			return OptionalPtr(nullptr);
		}

		T& Value() {
			UE_ASSERT_MSG(value != nullptr, "Cannot get value of none.");
			return *value;
		}

		bool IsSome() const {
			return static_cast<bool>(*this);
		}

		bool IsNone() const {
			return !IsSome();
		}

		operator bool() {
			return this->value != nullptr;
		}
	private:
		OptionalPtr(T* value)
			: value(value) {}

		T* value;
	};
}