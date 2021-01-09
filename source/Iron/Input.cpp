#include <map>
#include <GLFW/glfw3.h>
#include "Input.h"

// 0 - not pressed, 1 - just pressed, 2 - is held, 3 - just released
static std::map<KeyCode, int> pressedKeys;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // TODO: held, just released
    if (action == GLFW_PRESS)
        pressedKeys[(KeyCode)key] = 1;
    if (action == GLFW_RELEASE)
        pressedKeys[(KeyCode)key] = 0;
}

void Input::Init(GLFWwindow* window)
{
    glfwSetKeyCallback(window, keyCallback);
}

bool Input::IsKeyPressed(KeyCode code)
{
    return pressedKeys[code] > 0;
}

void Input::PollEvents()
{
    glfwPollEvents();
}
