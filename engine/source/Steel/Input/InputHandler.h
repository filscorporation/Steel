#pragma once

#include "ButtonStates.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

#include <string>
#include <glm/vec2.hpp>

class InputHandler
{
public:
    InputHandler() = default;
    virtual ~InputHandler() = default;

    static ButtonStates::ButtonState PressedKeys[MAX_KEY_CODE + 1];
    static ButtonStates::ButtonState PressedMouse[MAX_MOUSE_CODE + 1];
    static std::string TextInput;
    static glm::vec2 MousePosition;
    static glm::vec2 MouseScrollDelta;

    static bool KeyIsDirty;
    static bool MouseIsDirty;
    static bool ScrollDeltaIsDirty;
};
