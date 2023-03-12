#pragma once

#include "Steel/Input/InputHandler.h"

#include <android_native_app_glue.h>

class AndroidInputHandler : public InputHandler
{
public:
    AndroidInputHandler() = default;
    ~AndroidInputHandler() override = default;

    static void HandleInputEvent(AInputEvent* event);

    static void PollEvents();
};
