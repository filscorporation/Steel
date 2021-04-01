#pragma once

#include "DebugInfoWindow.h"

class Debug
{
public:
    static void Init();
    static void Terminate();

    static void EnableDebugWindow();
    static void DisableDebugWindow();
    static void Update();

    static void PauseUpdate();
    static void ContinueUpdate();

    static bool FreeCameraMode;

private:
    static DebugInfoWindow* debugInfoWindow;
};
