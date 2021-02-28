#pragma once

#include <cstdint>
#include "ScriptingCore.h"

namespace InternalCalls
{
    // Core
    void Application_Quit();
    bool Input_IsKeyPressed(int code);

    // Resources Manager
    uint64_t ResourcesManager_LoadImage(MonoString* path);
    void Sprite_SetAsSpriteSheet(uint64_t spriteID, int tileWidth, int tileHeight);
    int Sprite_GetWidth(uint64_t spriteID);
    int Sprite_GetHeight(uint64_t spriteID);
    uint64_t Animation_FromSpriteSheet(uint64_t spriteID, float length);
    bool Animation_GetLoop(uint64_t animationID);
    void Animation_SetLoop(uint64_t animationID, bool loop);

    // Time
    float Time_GetDeltaTime();
    float Time_GetTimeScale();
    void Time_SetTimeScale(float timeScale);

    // Log
    void Log_LogInfo(MonoString* message);
    void Log_LogWarning(MonoString* message);
    void Log_LogError(MonoString* message);
}
