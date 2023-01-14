#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "InputHandler.h"
#include "Steel/Rendering/Screen.h"
#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/UI/UIEvent.h"

#include <glm/glm.hpp>

struct GLFWwindow;

class Input
{
public:
    static void Init();

    static bool IsKeyPressed(KeyCodes::KeyCode code);
    static bool IsKeyJustPressed(KeyCodes::KeyCode code);
    static bool IsKeyJustReleased(KeyCodes::KeyCode code);
    static bool IsMouseButtonPressed(MouseCodes::MouseCode button);
    static bool IsMouseButtonJustPressed(MouseCodes::MouseCode button);
    static bool IsMouseButtonJustReleased(MouseCodes::MouseCode button);
    static glm::vec2 GetMousePosition();
    static glm::vec2 GetMouseDelta();
    static glm::vec2 GetMouseScrollDelta();

    static bool IsAnyKeyPressed();
    static bool IsAnyMouseButtonPressed();
    static bool IsAnyMouseButtonJustPressed();
    static bool IsAnyMouseButtonJustReleased();

    static UIEvent GetUIEvent();
    static void PollEvents();
    static void ReleaseAllEvents();

    static bool IgnoreEvents;

private:
    static glm::vec2 lastMousePosition;
};
