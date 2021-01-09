#pragma once

#include <GLM/glm.hpp>
#include "KeyCodes.h"
#include "MouseCodes.h"

class GLFWwindow;

class Input
{
public:
    // TODO: make internal
    static void Init(GLFWwindow* window);

    static bool IsKeyPressed(KeyCode code);
    //static bool IsMouseButtonPressed(MouseCode button);
    //static glm::vec2 GetMousePosition();
};