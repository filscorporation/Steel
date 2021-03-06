#include <GLFW/glfw3.h>
#include "Input.h"
#include "../Core/Application.h"
#include "../Physics/Physics.h"
#include "../Rendering/Camera.h"
#include "../Scripting/ScriptComponent.h"

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

Entity* Input::lastMouseOverCollider = nullptr;

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

void Input::SendMouseCallbacks()
{
    Camera* camera = Application::Instance->GetCurrentScene()->GetMainCamera();
    glm::vec2 worldMP = camera->ScreenToWorldPoint(GetMousePosition());
    auto hits = Physics::PointCast(worldMP);
    if (hits.empty())
    {
        // Nothing mouse is over
        if (lastMouseOverCollider != nullptr)
        {
            // We had stored last object mouse was over, send mouse exit event
            auto sr = lastMouseOverCollider->GetComponent<ScriptComponent>();
            if (sr != nullptr)
                sr->OnMouseExit();

            lastMouseOverCollider = nullptr;
        }

        return;
    }

    // Pick the nearest by z object
    float z, maxZ = hits[0]->Transform->GetPosition().z;
    int closestEntityID = 0;
    for (int i = 0; i < hits.size(); i++)
    {
        z = hits[i]->Transform->GetPosition().z;
        if (z > maxZ)
        {
            maxZ = z;
            closestEntityID = i;
        }
    }

    // Mouse is over some object
    if (lastMouseOverCollider != nullptr)
    {
        // We had stored last object mouse was over
        if (lastMouseOverCollider != hits[closestEntityID])
        {
            // Last object differs from new one, send exit to last, and enter to new
            auto sr = lastMouseOverCollider->GetComponent<ScriptComponent>();
            if (sr != nullptr)
                sr->OnMouseExit();

            lastMouseOverCollider = hits[closestEntityID];

            sr = lastMouseOverCollider->GetComponent<ScriptComponent>();
            if (sr != nullptr)
                sr->OnMouseEnter();
        }
        else
        {
            // We are still over the same object as last frame, send him over event
            auto sr = lastMouseOverCollider->GetComponent<ScriptComponent>();
            if (sr != nullptr)
                sr->OnMouseOver();
        }
    }
    else
    {
        // Nothing was stored, send enter to new one
        lastMouseOverCollider = hits[closestEntityID];

        auto sr = lastMouseOverCollider->GetComponent<ScriptComponent>();
        if (sr != nullptr)
            sr->OnMouseEnter();
    }

    auto sr = lastMouseOverCollider->GetComponent<ScriptComponent>();
    if (sr != nullptr)
    {
        if (IsAnyMouseButtonPressed())
            sr->OnMousePressed();
        if (IsAnyMouseButtonJustPressed())
            sr->OnMouseJustPressed();
        if (IsAnyMouseButtonJustReleased())
            sr->OnMouseJustReleased();
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
