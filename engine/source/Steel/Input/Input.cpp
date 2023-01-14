#include "Input.h"
#include "Steel/Core/Log.h"
#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    #include "Steel/Platform/Input/DesktopInputHandler.h"
#endif
#if defined PLATFORM_ANDROID
    #include "Steel/Platform/Input/AndroidInputHandler.h"
#endif

glm::vec2 Input::lastMousePosition = { };

bool Input::IgnoreEvents = false;

void Input::Init()
{
    for (int i = 0; i < MAX_KEY_CODE + 1; ++i)
    {
        InputHandler::PressedKeys[i] = ButtonStates::NotPressed;
    }
    for (int i = 0; i < MAX_MOUSE_CODE + 1; ++i)
    {
        InputHandler::PressedMouse[i] = ButtonStates::NotPressed;
    }
}

bool Input::IsKeyPressed(KeyCodes::KeyCode code)
{
    return !IgnoreEvents && InputHandler::PressedKeys[code] == ButtonStates::JustPressed || InputHandler::PressedKeys[code] == ButtonStates::IsHeld;
}

bool Input::IsKeyJustPressed(KeyCodes::KeyCode code)
{
    return !IgnoreEvents && InputHandler::PressedKeys[code] == ButtonStates::JustPressed;
}

bool Input::IsKeyJustReleased(KeyCodes::KeyCode code)
{
    return !IgnoreEvents && InputHandler::PressedKeys[code] == ButtonStates::JustReleased;
}

bool Input::IsMouseButtonPressed(MouseCodes::MouseCode button)
{
    return !IgnoreEvents && InputHandler::PressedMouse[button] == ButtonStates::JustPressed || InputHandler::PressedMouse[button] == ButtonStates::IsHeld;
}

bool Input::IsMouseButtonJustPressed(MouseCodes::MouseCode button)
{
    return !IgnoreEvents && InputHandler::PressedMouse[button] == ButtonStates::JustPressed;
}

bool Input::IsMouseButtonJustReleased(MouseCodes::MouseCode button)
{
    return !IgnoreEvents && InputHandler::PressedMouse[button] == ButtonStates::JustReleased;
}

glm::vec2 Input::GetMousePosition()
{
    return Screen::Transform(InputHandler::MousePosition);
}

glm::vec2 Input::GetMouseDelta()
{
    return Screen::Transform(InputHandler::MousePosition - lastMousePosition);
}

glm::vec2 Input::GetMouseScrollDelta()
{
    return IgnoreEvents ? glm::vec2(0.0f, 0.0f) : InputHandler::MouseScrollDelta;
}

void CleanKeys()
{
    for (int i = 0; i < MAX_KEY_CODE + 1; ++i)
    {
        if (InputHandler::PressedKeys[i] == ButtonStates::JustPressed)
            InputHandler::PressedKeys[i] = ButtonStates::IsHeld;
        if (InputHandler::PressedKeys[i] == ButtonStates::JustReleased)
            InputHandler::PressedKeys[i] = ButtonStates::NotPressed;
    }
}

void CleanMouse()
{
    for (int i = 0; i < MAX_MOUSE_CODE + 1; ++i)
    {
        if (InputHandler::PressedMouse[i] == ButtonStates::JustPressed)
            InputHandler::PressedMouse[i] = ButtonStates::IsHeld;
        if (InputHandler::PressedMouse[i] == ButtonStates::JustReleased)
            InputHandler::PressedMouse[i] = ButtonStates::NotPressed;
    }
}

void CleanScrollDelta()
{
    InputHandler::MouseScrollDelta.x = 0;
    InputHandler::MouseScrollDelta.y = 0;
}

UIEvent Input::GetUIEvent()
{
    // Input can only fill event data, event type flags will be calculated by event handlers
    UIEvent uiEvent{};
    uiEvent.Used = false;
    uiEvent.MousePosition = GetMousePosition();
    uiEvent.MouseDelta = GetMouseDelta();
    uiEvent.ScrollDelta = InputHandler::MouseScrollDelta;
    uiEvent.LeftMouseButtonState = InputHandler::PressedMouse[MouseCodes::ButtonLeft];
    uiEvent.RightMouseButtonState = InputHandler::PressedMouse[MouseCodes::ButtonRight];
    uiEvent.MiddleMouseButtonState = InputHandler::PressedMouse[MouseCodes::ButtonMiddle];
    uiEvent.AnyKey = IsAnyKeyPressed();
    uiEvent.InputString = InputHandler::TextInput;

    return uiEvent;
}

void Input::PollEvents()
{
    if (InputHandler::KeyIsDirty)
    {
        InputHandler::KeyIsDirty = false;
        CleanKeys();
    }
    if (InputHandler::MouseIsDirty)
    {
        InputHandler::MouseIsDirty = false;
        CleanMouse();
    }
    if (InputHandler::ScrollDeltaIsDirty)
    {
        InputHandler::ScrollDeltaIsDirty = true;
        CleanScrollDelta();
    }
    InputHandler::TextInput.clear();

    lastMousePosition = InputHandler::MousePosition;

#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
    DesktopInputHandler::PollEvents();
#endif
#if defined PLATFORM_ANDROID
    AndroidInputHandler::PollEvents();
#endif
}

void Input::ReleaseAllEvents()
{
    // This fixes glfw bug, when there is no release event if you move or resize window
    for (auto& key : InputHandler::PressedKeys)
    {
        if (key == ButtonStates::JustPressed || key == ButtonStates::IsHeld)
        {
            key = ButtonStates::JustReleased;
            InputHandler::KeyIsDirty = true;
        }
    }

    for (auto& mouse : InputHandler::PressedMouse)
    {
        if (mouse == ButtonStates::JustPressed || mouse == ButtonStates::IsHeld)
        {
            mouse = ButtonStates::JustReleased;
            InputHandler::MouseIsDirty = true;
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