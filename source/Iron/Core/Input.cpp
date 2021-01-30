#include <map>
#include <GLFW/glfw3.h>
#include "Input.h"

// 0 - not pressed, 1 - just pressed, 2 - is held, 3 - just released
static std::map<KeyCodes::KeyCode, int> pressedKeys;
static std::map<MouseCodes::MouseCode, int> pressedMouse;
static glm::vec2 mousePosition;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // TODO: held, just released
    if (action == GLFW_PRESS)
        pressedKeys[(KeyCodes::KeyCode)key] = 1;
    if (action == GLFW_RELEASE)
        pressedKeys[(KeyCodes::KeyCode)key] = 0;
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    // TODO: held, just released
    if (action == GLFW_PRESS)
        pressedMouse[(MouseCodes::MouseCode)button] = 1;
    if (action == GLFW_RELEASE)
        pressedMouse[(MouseCodes::MouseCode)button] = 0;
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    mousePosition.x = xpos;
    mousePosition.y = ypos;
}

void Input::Init(GLFWwindow* window)
{
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
}

bool Input::IsKeyPressed(KeyCodes::KeyCode code)
{
    return pressedKeys[code] > 0;
}

bool Input::IsMouseButtonPressed(MouseCodes::MouseCode button)
{
    return pressedMouse[button] > 0;
}

glm::vec2 Input::GetMousePosition()
{
    return mousePosition;
}

void Input::PollEvents()
{
    glfwPollEvents();
}
