#pragma once

#include <glm/vec2.hpp>

#include "InputCode.h"
#include "Gamepad.h"

namespace UniverseEngine {
    enum class CursorMode {
        ENABLED,
        DISABLED
    };

    class Input {
    public:
        bool GetKey(KeyCode keyCode) const;
        bool GetKeyDown(KeyCode keyCode) const;
        bool GetMouseButton(MouseButton mouseButton) const;
        bool GetMouseButtonDown(MouseButton mouseButton) const;

        const glm::vec2& GetMousePosition() const;
        glm::vec2 GetMouseDelta() const;

        CursorMode GetCursorMode() const;
        void SetCursorMode(CursorMode cursorMode);

        const Gamepad& GetGamepad(GamepadIndex index = GamepadIndex::GAMEPAD_1) const;

    private:
        friend class Engine;
        friend class Window;
        Input();

        void Update();
        void EditorBar();
        void Editor();

        static const size_t MAX_KEYS = 512;
        static const size_t MAX_MOUSE_BUTTONS = 8;
        bool keys[MAX_KEYS];
        bool oldKeys[MAX_KEYS];
        bool mouseButtons[MAX_MOUSE_BUTTONS];
        bool oldMouseButtons[MAX_MOUSE_BUTTONS];

        glm::vec2 mousePosition;
        glm::vec2 oldMousePosition;
        CursorMode cursorMode;

        Gamepad gamepads[MAX_GAMEPADS];
    };
}