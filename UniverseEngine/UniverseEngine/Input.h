#pragma once

#include <glm/vec2.hpp>

#include "InputCode.h"

namespace UniverseEngine {
    class Input {
    public:
        bool GetKey(KeyCode keyCode) const;
        bool GetKeyDown(KeyCode keyCode) const;
        bool GetMouseButton(MouseButton mouseButton) const;
        bool GetMouseButtonDown(MouseButton mouseButton) const;

        const glm::vec2& GetMousePosition() const;
        const glm::vec2& GetMouseDelta() const;

    private:
        friend class Engine;
        friend class Window;
        Input();

        void Update();

        static const size_t MAX_KEYS = 512;
        static const size_t MAX_MOUSE_BUTTONS = 8;
        bool keys[MAX_KEYS];
        bool oldKeys[MAX_KEYS];
        bool mouseButtons[MAX_MOUSE_BUTTONS];
        bool oldMouseButtons[MAX_MOUSE_BUTTONS];

        glm::vec2 mousePosition;
        glm::vec2 mouseDelta;
    };
}