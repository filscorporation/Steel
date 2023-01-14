#pragma once

#include "Steel/Input/InputHandler.h"

struct GLFWwindow;

class DesktopInputHandler : public InputHandler
{
public:
    DesktopInputHandler() = default;
    ~DesktopInputHandler() override = default;

    static void Init(GLFWwindow* window);
    static void PollEvents();
};
