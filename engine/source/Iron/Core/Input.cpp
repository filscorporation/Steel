#include <GLFW/glfw3.h>

#include "Input.h"
#include "ButtonStates.h"
#include "../Core/Application.h"
#include "../Physics/Physics.h"
#include "../Scripting/ScriptComponent.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/Transformation.h"

ButtonStates::ButtonState pressedKeys[MAX_KEY_CODE + 1];
ButtonStates::ButtonState pressedMouse[MAX_MOUSE_CODE + 1];
glm::vec2 mousePosition;
glm::vec2 lastMousePosition;
glm::vec2 mouseScrollDelta;
bool keyIsDirty = false;
bool mouseIsDirty = false;
bool scrollDeltaIsDirty = false;

EntityID Input::lastMouseOverCollider = NULL_ENTITY;

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
    lastMousePosition = mousePosition;

    mousePosition.x = (float)xPos;
    // GLFW coordinate system is upside down by Y-axis
    mousePosition.y = (float)Screen::GetHeight() - (float)yPos;
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

UIEvent Input::GetUIEvent()
{
    // Input can only fill event data, event type flags will be calculated by event handlers
    UIEvent uiEvent{};
    uiEvent.Used = false;
    uiEvent.MousePosition = mousePosition;
    uiEvent.MouseDelta = mousePosition - lastMousePosition;
    uiEvent.ScrollDelta = mouseScrollDelta;
    uiEvent.LeftMouseButtonState = pressedMouse[MouseCodes::ButtonLeft];
    uiEvent.RightMouseButtonState = pressedMouse[MouseCodes::ButtonRight];

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

    glfwPollEvents();
}

void Input::SendMouseCallbacks()
{
    Camera& camera = Application::Instance->GetCurrentScene()->GetMainCamera();
    glm::vec2 worldMP = camera.ScreenToWorldPoint(GetMousePosition());
    auto hits = Physics::PointCast(worldMP);
    if (hits.empty())
    {
        // Nothing mouse is over
        if (lastMouseOverCollider != NULL_ENTITY)
        {
            // We had stored last object mouse was over, send mouse exit event
            if (HasComponentS<ScriptComponent>(lastMouseOverCollider))
                GetComponentS<ScriptComponent>(lastMouseOverCollider).OnMouseExit();

            lastMouseOverCollider = NULL_ENTITY;
        }

        return;
    }

    // Pick the nearest by z object
    float z, maxZ = GetComponentS<Transformation>(hits[0]).GetPosition().z;
    int closestEntityID = 0;
    for (size_t i = 0; i < hits.size(); i++)
    {
        z = GetComponentS<Transformation>(hits[i]).GetPosition().z;
        if (z > maxZ)
        {
            maxZ = z;
            closestEntityID = i;
        }
    }

    // Mouse is over some object
    if (lastMouseOverCollider != NULL_ENTITY)
    {
        // We had stored last object mouse was over
        if (lastMouseOverCollider != hits[closestEntityID])
        {
            // Last object differs from new one, send exit to last, and enter to new
            if (HasComponentS<ScriptComponent>(lastMouseOverCollider))
                GetComponentS<ScriptComponent>(lastMouseOverCollider).OnMouseExit();

            lastMouseOverCollider = hits[closestEntityID];

            if (HasComponentS<ScriptComponent>(lastMouseOverCollider))
                GetComponentS<ScriptComponent>(lastMouseOverCollider).OnMouseEnter();
        }
        else
        {
            // We are still over the same object as last frame, send him over event
            if (HasComponentS<ScriptComponent>(lastMouseOverCollider))
                GetComponentS<ScriptComponent>(lastMouseOverCollider).OnMouseOver();
        }
    }
    else
    {
        // Nothing was stored, send enter to new one
        lastMouseOverCollider = hits[closestEntityID];

        if (HasComponentS<ScriptComponent>(lastMouseOverCollider))
            GetComponentS<ScriptComponent>(lastMouseOverCollider).OnMouseEnter();
    }

    if (HasComponentS<ScriptComponent>(lastMouseOverCollider))
    {
        auto& sr = GetComponentS<ScriptComponent>(lastMouseOverCollider);

        if (IsAnyMouseButtonJustPressed())
            sr.OnMouseJustPressed();
        if (IsAnyMouseButtonPressed())
            sr.OnMousePressed();
        if (IsAnyMouseButtonJustReleased())
            sr.OnMouseJustReleased();
    }
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
