#include "Input.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Engine.h"

namespace UniverseEngine {
    Input::Input()
        : keys{}, oldKeys{}, mouseButtons{}, oldMouseButtons{}, mousePosition{}, mouseDelta{} {
    }

    bool Input::GetKey(KeyCode keyCode) const {
        return this->keys[static_cast<size_t>(keyCode)];
    }

    bool Input::GetKeyDown(KeyCode keyCode) const {
        return this->keys[static_cast<size_t>(keyCode)] &&
               !this->oldKeys[static_cast<size_t>(keyCode)];
    }

    bool Input::GetMouseButton(MouseButton mouseButton) const {
        return this->mouseButtons[static_cast<size_t>(mouseButton)];
    }

    bool Input::GetMouseButtonDown(MouseButton mouseButton) const {
        return this->mouseButtons[static_cast<size_t>(mouseButton)] &&
               !this->oldMouseButtons[static_cast<size_t>(mouseButton)];
    }

    const glm::vec2& Input::GetMousePosition() const {
        return this->mousePosition;
    }

    const glm::vec2& Input::GetMouseDelta() const {
        return this->mouseDelta;
    }

    void Input::Update() {
        memcpy(this->oldKeys, this->keys, sizeof(bool) * MAX_KEYS);
        memcpy(this->oldMouseButtons, this->mouseButtons, sizeof(bool) * MAX_MOUSE_BUTTONS);

        memset(this->keys, 0, sizeof(bool) * MAX_KEYS);
        memset(this->mouseButtons, 0, sizeof(bool) * MAX_MOUSE_BUTTONS);
    }
}  // namespace UniverseEngine