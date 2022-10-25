#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Steel/Scripting/ScriptingCore.h"

namespace CoreInternalCalls
{
    // Core
    void Application_Quit();
    MonoString* Application_RuntimePath();
    MonoString* Application_DataPath();

    // Input
    bool Input_IsKeyPressed(int code);
    void Input_GetMousePosition(glm::vec2* position);
    void Input_GetMouseScrollDelta(glm::vec2* delta);
    bool Input_IsKeyJustPressed(int code);
    bool Input_IsKeyJustReleased(int code);
    bool Input_IsMousePressed(int code);
    bool Input_IsMouseJustPressed(int code);
    bool Input_IsMouseJustReleased(int code);

    // Scene Manager
    ResourceID SceneManager_CreateNewScene(MonoString* name);
    void SceneManager_SetActiveScene(ResourceID sceneID);
    void SceneManager_SetActiveScene2(MonoString* path);
    ResourceID SceneManager_GetActiveScene();
    MonoString* Scene_GetName(ResourceID sceneID);

    // Time
    float Time_GetDeltaTime();
    float Time_GetUnscaledDeltaTime();
    float Time_GetTimeSinceStartup();
    uint64_t Time_GetFrameCount();
    float Time_GetFixedDeltaTime();
    void Time_SetFixedDeltaTime(float fixedDeltaTime);
    float Time_GetTimeScale();
    void Time_SetTimeScale(float timeScale);

    // Screen
    int Screen_GetWidth();
    void Screen_SetWidth(int width);
    int Screen_GetHeight();
    void Screen_SetHeight(int height);
    bool Screen_GetFullscreen();
    void Screen_SetFullscreen(bool fullscreen);
    bool Screen_GetIsMinimized();
    glm::vec4 Screen_GetColor();
    void Screen_SetColor(glm::vec4 color);

    // Log
    void Log_LogDebug(MonoString* message);
    void Log_LogInfo(MonoString* message);
    void Log_LogWarning(MonoString* message);
    void Log_LogError(MonoString* message);

    // Physics
    void Physics_Simulate(float deltaTime);
    MonoArray* Physics_PointCast(glm::vec2* point);
    MonoArray* Physics_AABBCast(glm::vec2* min, glm::vec2* max);
    MonoArray* Physics_LineCast(glm::vec2* pointA, glm::vec2* pointB);

    // Random
    float Random_NextFloat(float min, float max);
    int Random_NextInt(int min, int max);
    float Random_PerlinNoise(float x, float y);
}
