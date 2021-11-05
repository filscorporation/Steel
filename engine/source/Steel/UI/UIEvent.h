#pragma once

#include "glm/vec2.hpp"
#include <string>

#include "../Core/ButtonStates.h"

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

    inline UIEventType operator|(UIEventType a, UIEventType b)
    {
        return static_cast<UIEventType>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline UIEventType operator&(UIEventType a, UIEventType b)
    {
        return static_cast<UIEventType>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }
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
