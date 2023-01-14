#include "DesktopInputHandler.h"
#include "Steel/Rendering/Screen.h"

#include <GLFW/glfw3.h>

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    int ikey = (KeyCodes::KeyCode)key;

    if (action == GLFW_PRESS)
    {
        InputHandler::PressedKeys[ikey] = ButtonStates::JustPressed;
        InputHandler::KeyIsDirty = true;
    }
    if (action == GLFW_RELEASE)
    {
        InputHandler::PressedKeys[ikey] = ButtonStates::JustReleased;
        InputHandler::KeyIsDirty = true;
    }
}

void TextInputCallback(GLFWwindow* window, uint32_t codepoint)
{
    InputHandler::TextInput.push_back((char)codepoint);
}

void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    int buttonIndex = (MouseCodes::MouseCode)button;

    if (action == GLFW_PRESS)
    {
        InputHandler::PressedMouse[buttonIndex] = ButtonStates::JustPressed;
        InputHandler::MouseIsDirty = true;
    }
    if (action == GLFW_RELEASE)
    {
        InputHandler::PressedMouse[buttonIndex] = ButtonStates::JustReleased;
        InputHandler::MouseIsDirty = true;
    }
}

void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
    InputHandler::MousePosition.x = (float)xPos;
    // GLFW coordinate system is upside down by Y-axis
    InputHandler::MousePosition.y = Screen::InvertY((float)yPos);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    InputHandler::MouseScrollDelta.x = (float)xOffset;
    InputHandler::MouseScrollDelta.y = (float)yOffset;

    InputHandler::ScrollDeltaIsDirty = true;
}

void DesktopInputHandler::Init(GLFWwindow* window)
{
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCharCallback(window, TextInputCallback);
    glfwSetMouseButtonCallback(window, MouseCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

void DesktopInputHandler::PollEvents()
{
    glfwPollEvents();
}
