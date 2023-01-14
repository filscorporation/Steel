#pragma once

#include "Steel/Input/InputHandler.h"

class AndroidInputHandler : public InputHandler
{
public:
    AndroidInputHandler() = default;
    ~AndroidInputHandler() override = default;

    static void PollEvents();
};
