#pragma once

#include "InputCode.h"

namespace UniverseEngine {
    class Input;

    static const size_t MAX_GAMEPADS = 4;
    enum class GamepadIndex { GAMEPAD_1, GAMEPAD_2, GAMEPAD_3, GAMEPAD_4 };

    class Gamepad {
    public:
        GamepadIndex Index() const;
        bool Connected() const;

        float GetAxis(GamepadAxis axis) const;
        bool GetButton(GamepadButton button) const;
        bool GetButtonDown(GamepadButton button) const;

    private:
        friend class Input;
        Gamepad(GamepadIndex index);

        void Update();

        GamepadIndex index;
        bool connected;

        static const size_t NUM_AXIS = 6;
        static const size_t NUM_BUTTONS = 15;
        float axis[NUM_AXIS];
        bool buttons[NUM_BUTTONS];
        bool oldButtons[NUM_BUTTONS];
    };
}  // namespace UniverseEngine