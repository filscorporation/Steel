#include <GLFW/glfw3.h>
#include "Input.h"

namespace ButtonStates
{
    enum ButtonState
    {
        NotPressed = 0,
        JustPressed = 1,
        IsHeld = 2,
        JustReleased = 3,
    };
}

ButtonStates::ButtonState pressedKeys[MAX_KEY_CODE + 1];
ButtonStates::ButtonState pressedMouse[MAX_MOUSE_CODE + 1];
glm::vec2 mousePosition;
glm::vec2 mouseScrollDelta;
bool keyIsDirty = false;
bool mouseIsDirty = false;
bool scrollDeltaIsDirty = false;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    int ikey = (KeyCodes::KeyCode)key;

    if (action == GLFW_PRESS)
    {
        pressedKeys[ikey] = ButtonStates::JustPressed;
        keyIsDirty = true;
    }
    if (action == GLFW_RELEASE)
    {
        pressedKeys[ikey] = ButtonStates::JustReleased;
        keyIsDirty = true;
    }
}

void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    int ibutton = (MouseCodes::MouseCode)button;

    if (action == GLFW_PRESS)
    {
        pressedMouse[ibutton] = ButtonStates::JustPressed;
        mouseIsDirty = true;
    }
    if (action == GLFW_RELEASE)
    {
        pressedMouse[ibutton] = ButtonStates::JustReleased;
        mouseIsDirty = true;
    }
}

void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    mousePosition.x = xPos;
    mousePosition.y = yPos;
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    mouseScrollDelta.x = xOffset;
    mouseScrollDelta.y = yOffset;

    scrollDeltaIsDirty = true;
}

void Input::Init(GLFWwindow* window)
{
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    for (int i = 0; i < MAX_KEY_CODE + 1; ++i)
    {
        pressedKeys[i] = ButtonStates::NotPressed;
    }
    for (int i = 0; i < MAX_MOUSE_CODE + 1; ++i)
    {
        pressedMouse[i] = ButtonStates::NotPressed;
    }
}

bool Input::IsKeyPressed(KeyCodes::KeyCode code)
{
    return pressedKeys[code] == ButtonStates::JustPressed || pressedKeys[code] == ButtonStates::IsHeld;
}

bool Input::IsKeyJustPressed(KeyCodes::KeyCode code)
{
    return pressedKeys[code] == ButtonStates::JustPressed;
}

bool Input::IsKeyJustReleased(KeyCodes::KeyCode code)
{
    return pressedKeys[code] == ButtonStates::JustReleased;
}

bool Input::IsMouseButtonPressed(MouseCodes::MouseCode button)
{
    return pressedMouse[button] == ButtonStates::JustPressed || pressedMouse[button] == ButtonStates::IsHeld;
}

bool Input::IsMouseButtonJustPressed(MouseCodes::MouseCode button)
{
    return pressedMouse[button] == ButtonStates::JustPressed;
}

bool Input::IsMouseButtonJustReleased(MouseCodes::MouseCode button)
{
    return pressedMouse[button] == ButtonStates::JustReleased;
}

glm::vec2 Input::GetMousePosition()
{
    return mousePosition;
}

glm::vec2 Input::GetMouseScrollDelta()
{
    return mouseScrollDelta;
}

void CleanKeys()
{
    for (int i = 0; i < MAX_KEY_CODE + 1; ++i)
    {
        if (pressedKeys[i] == ButtonStates::JustPressed)
            pressedKeys[i] = ButtonStates::IsHeld;
        if (pressedKeys[i] == ButtonStates::JustReleased)
            pressedKeys[i] = ButtonStates::NotPressed;
    }
}

void CleanMouse()
{
    for (int i = 0; i < MAX_MOUSE_CODE + 1; ++i)
    {
        if (pressedMouse[i] == ButtonStates::JustPressed)
            pressedMouse[i] = ButtonStates::IsHeld;
        if (pressedMouse[i] == ButtonStates::JustReleased)
            pressedMouse[i] = ButtonStates::NotPressed;
    }
}

void CleanScrollDelta()
{
    mouseScrollDelta.x = 0;
    mouseScrollDelta.y = 0;
}

void Input::PollEvents()
{
    if (keyIsDirty)
    {
        keyIsDirty = false;
        CleanKeys();
    }
    if (mouseIsDirty)
    {
        mouseIsDirty = false;
        CleanMouse();
    }
    if (scrollDeltaIsDirty)
    {
        scrollDeltaIsDirty = true;
        CleanScrollDelta();
    }

    glfwPollEvents();
}
