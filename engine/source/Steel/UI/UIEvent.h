#pragma once

#include "Steel/Input/ButtonStates.h"
#include "Steel/Common/FlagType.h"

#include <glm/vec2.hpp>
#include <string>

namespace UIEventTypes
{
    enum UIEventType
    {
        MouseEnter                = 1 << 0,
        MouseExit                 = 1 << 1,
        MouseOver                 = 1 << 2,
        MouseJustPressed          = 1 << 3,
        MousePressed              = 1 << 4,
        MouseJustReleased         = 1 << 5,
        MouseJustPressedAnywhere  = 1 << 6,
        MousePressedAnywhere      = 1 << 7,
        MouseJustReleasedAnywhere = 1 << 8,
        MouseDragBegin            = 1 << 9,
        MouseDrag                 = 1 << 10,
        MouseDragEnd              = 1 << 11,
        ScrollDelta               = 1 << 12,
        KeyInput                  = 1 << 13,
        TextInput                 = 1 << 14,
    };

    FLAG_TYPE(UIEventType)
}

struct UIEvent
{
    bool Used = false;
    // If not 0, then event got 'clipped' by UI clipping planes
    int ClippingDepth = 0;

    glm::vec2 MousePosition;
    glm::vec2 MouseDelta;
    glm::vec2 ScrollDelta;
    ButtonStates::ButtonState LeftMouseButtonState;
    ButtonStates::ButtonState RightMouseButtonState;
    ButtonStates::ButtonState MiddleMouseButtonState;
    bool AnyKey = false;
    std::string InputString;
};
