#include <GLFW/glfw3.h>

#include "Input.h"
#include "Log.h"

ButtonStates::ButtonState pressedKeys[MAX_KEY_CODE + 1];
ButtonStates::ButtonState pressedMouse[MAX_MOUSE_CODE + 1];
std::string textInput;
glm::vec2 mousePosition;
glm::vec2 lastMousePosition;
glm::vec2 mouseScrollDelta;
bool keyIsDirty = false;
bool mouseIsDirty = false;
bool scrollDeltaIsDirty = false;

bool Input::IgnoreEvents = false;

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

void TextInputCallback(GLFWwindow* window, uint32_t codepoint)
{
    textInput.push_back((char)codepoint);
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
    mousePosition.x = (float)xPos;
    // GLFW coordinate system is upside down by Y-axis
    mousePosition.y = Screen::InvertY((float)yPos);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    mouseScrollDelta.x = (float)xOffset;
    mouseScrollDelta.y = (float)yOffset;

    scrollDeltaIsDirty = true;
}

void Input::Init(GLFWwindow* window)
{
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCharCallback(window, TextInputCallback);
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
    return !IgnoreEvents && pressedKeys[code] == ButtonStates::JustPressed || pressedKeys[code] == ButtonStates::IsHeld;
}

bool Input::IsKeyJustPressed(KeyCodes::KeyCode code)
{
    return !IgnoreEvents && pressedKeys[code] == ButtonStates::JustPressed;
}

bool Input::IsKeyJustReleased(KeyCodes::KeyCode code)
{
    return !IgnoreEvents && pressedKeys[code] == ButtonStates::JustReleased;
}

bool Input::IsMouseButtonPressed(MouseCodes::MouseCode button)
{
    return !IgnoreEvents && pressedMouse[button] == ButtonStates::JustPressed || pressedMouse[button] == ButtonStates::IsHeld;
}

bool Input::IsMouseButtonJustPressed(MouseCodes::MouseCode button)
{
    return !IgnoreEvents && pressedMouse[button] == ButtonStates::JustPressed;
}

bool Input::IsMouseButtonJustReleased(MouseCodes::MouseCode button)
{
    return !IgnoreEvents && pressedMouse[button] == ButtonStates::JustReleased;
}

glm::vec2 Input::GetMousePosition()
{
    return Screen::Transform(mousePosition);
}

glm::vec2 Input::GetMouseDelta()
{
    return Screen::Transform(mousePosition - lastMousePosition);
}

glm::vec2 Input::GetMouseScrollDelta()
{
    return IgnoreEvents ? glm::vec2(0.0f, 0.0f) : mouseScrollDelta;
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

UIEvent Input::GetUIEvent()
{
    // Input can only fill event data, event type flags will be calculated by event handlers
    UIEvent uiEvent{};
    uiEvent.Used = false;
    uiEvent.MousePosition = GetMousePosition();
    uiEvent.MouseDelta = GetMouseDelta();
    uiEvent.ScrollDelta = mouseScrollDelta;
    uiEvent.LeftMouseButtonState = pressedMouse[MouseCodes::ButtonLeft];
    uiEvent.RightMouseButtonState = pressedMouse[MouseCodes::ButtonRight];
    uiEvent.MiddleMouseButtonState = pressedMouse[MouseCodes::ButtonMiddle];
    uiEvent.AnyKey = IsAnyKeyPressed();
    uiEvent.InputString = textInput;

    return uiEvent;
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
    textInput.clear();

    lastMousePosition = mousePosition;
    glfwPollEvents();
}

void Input::ReleaseAllEvents()
{
    // This fixes glfw bug, when there is no realse event if you move or resize window
    for (auto& key : pressedKeys)
    {
        if (key == ButtonStates::JustPressed || key == ButtonStates::IsHeld)
        {
            key = ButtonStates::JustReleased;
            keyIsDirty = true;
        }
    }

    for (auto& mouse : pressedMouse)
    {
        if (mouse == ButtonStates::JustPressed || mouse == ButtonStates::IsHeld)
        {
            mouse = ButtonStates::JustReleased;
            mouseIsDirty = true;
        }
    }
}

bool Input::IsAnyKeyPressed()
{
    for (int i = 0; i < MAX_KEY_CODE + 1; ++i)
    {
        if (IsKeyPressed((KeyCodes::KeyCode)i) ||
            IsKeyJustPressed((KeyCodes::KeyCode)i) ||
            IsKeyJustReleased((KeyCodes::KeyCode)i))
            return true;
    }
    return false;
}

bool Input::IsAnyMouseButtonPressed()
{
    for (int i = 0; i < MAX_MOUSE_CODE + 1; ++i)
    {
        if (IsMouseButtonPressed((MouseCodes::MouseCode)i)) return true;
    }
    return false;
}

bool Input::IsAnyMouseButtonJustPressed()
{
    for (int i = 0; i < MAX_MOUSE_CODE + 1; ++i)
    {
        if (IsMouseButtonJustPressed((MouseCodes::MouseCode)i)) return true;
    }
    return false;
}

bool Input::IsAnyMouseButtonJustReleased()
{
    for (int i = 0; i < MAX_MOUSE_CODE + 1; ++i)
    {
        if (IsMouseButtonJustReleased((MouseCodes::MouseCode)i)) return true;
    }
    return false;
}