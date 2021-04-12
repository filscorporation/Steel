#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "ScriptingCore.h"
#include "../Core/ResourceID.h"

namespace CoreInternalCalls
{
    // Core
    void Application_Quit();
    int Application_GetState();
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

    // Resources Manager
    ResourceID ResourcesManager_LoadImage(MonoString* path);
    ResourceID ResourcesManager_LoadAudioTrack(MonoString* path);
    float AudioTrack_GetLength(ResourceID audioTrackID);
    void Sprite_SetAsSpriteSheet(ResourceID spriteID, int tileWidth, int tileHeight);
    int Sprite_GetWidth(ResourceID spriteID);
    int Sprite_GetHeight(ResourceID spriteID);
    int Sprite_GetPixelsPerUnit(ResourceID spriteID);
    void Sprite_SetPixelsPerUnit(ResourceID spriteID, int pixelsPerUnit);
    ResourceID Animation_FromSpriteSheet(ResourceID spriteID, float length);
    ResourceID Animation_FromSprites(MonoArray* spritesIDs, float length);
    float Animation_GetLength(ResourceID animationID);
    bool Animation_GetLoop(ResourceID animationID);
    void Animation_SetLoop(ResourceID animationID, bool loop);

    // Time
    float Time_GetDeltaTime();
    float Time_GetTimeScale();
    void Time_SetTimeScale(float timeScale);

    // Screen
    int Screen_GetWidth();
    void Screen_SetWidth(int width);
    int Screen_GetHeight();
    void Screen_SetHeight(int height);
    bool Screen_GetFullscreen();
    void Screen_SetFullscreen(bool fullscreen);
    glm::vec4 Screen_GetColor();
    void Screen_SetColor(glm::vec4 color);

    // Log
    void Log_LogInfo(MonoString* message);
    void Log_LogWarning(MonoString* message);
    void Log_LogError(MonoString* message);

    // Debug
    void Debug_EnableDebugInfoWindow();
    void Debug_DisableDebugInfoWindow();

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
