#include "Gamepad.h"

#include <GLFW/glfw3.h>

#include "Logging.h"

namespace UniverseEngine {
    int IndexToGlfw(GamepadIndex index) {
        switch (index) {
            case GamepadIndex::GAMEPAD_1:
                return GLFW_JOYSTICK_1;
            case GamepadIndex::GAMEPAD_2:
                return GLFW_JOYSTICK_2;
            case GamepadIndex::GAMEPAD_3:
                return GLFW_JOYSTICK_3;
            default:
                return GLFW_JOYSTICK_4;
        }
    }

    Gamepad::Gamepad(GamepadIndex index)
        : index(index), connected(false), axis{}, buttons{}, oldButtons{} {
    }

    GamepadIndex Gamepad::Index() const {
        return this->index;
    }

    bool Gamepad::Connected() const {
        return this->connected;
    }

    float Gamepad::GetAxis(GamepadAxis axis) const {
        return this->axis[static_cast<size_t>(axis)];
    }

    bool Gamepad::GetButton(GamepadButton button) const {
        return this->buttons[static_cast<size_t>(button)];
    }

    bool Gamepad::GetButtonDown(GamepadButton button) const {
        return this->buttons[static_cast<size_t>(button)] &&
               !this->oldButtons[static_cast<size_t>(button)];
    }

    void Gamepad::Update() {
        int jid = IndexToGlfw(this->index);

        this->connected = (glfwJoystickPresent(jid) == GLFW_TRUE);

        int count;
        const float* axes = glfwGetJoystickAxes(jid, &count);
        UE_ASSERT_MSG(count <= NUM_AXIS, "Gamepad exceeds max axis.");
        for (size_t i = 0; i < count; i++) {
            this->axis[i] = axis[i];
        }

        memcpy(this->oldButtons, this->buttons, sizeof(bool) * NUM_BUTTONS);

        const unsigned char* buttons = glfwGetJoystickButtons(jid, &count);
        UE_ASSERT_MSG(count <= NUM_BUTTONS, "Gamepad exceeds max buttons.");
        for (size_t i = 0; i < count; i++) {
            this->buttons[i] = (buttons[i] == GLFW_PRESS);
        }
    }
}  // namespace UniverseEngine