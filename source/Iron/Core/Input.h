#pragma once

#include <glm/glm.hpp>
#include "KeyCodes.h"
#include "MouseCodes.h"
#include "../Rendering/Screen.h"
#include "../Scene/Entity.h"

struct GLFWwindow;

class Input
{
public:
    static bool IsKeyPressed(KeyCodes::KeyCode code);
    static bool IsKeyJustPressed(KeyCodes::KeyCode code);
    static bool IsKeyJustReleased(KeyCodes::KeyCode code);
    static bool IsMouseButtonPressed(MouseCodes::MouseCode button);
    static bool IsMouseButtonJustPressed(MouseCodes::MouseCode button);
    static bool IsMouseButtonJustReleased(MouseCodes::MouseCode button);
    static glm::vec2 GetMousePosition();
    static glm::vec2 GetMouseScrollDelta();

    static void PollEvents();
    static void SendMouseCallbacks();

private:
    static void Init(GLFWwindow* window);

    static bool IsAnyMouseButtonPressed();
    static bool IsAnyMouseButtonJustPressed();
    static bool IsAnyMouseButtonJustReleased();

    static Entity* lastMouseOverCollider;

    friend class Screen;
};