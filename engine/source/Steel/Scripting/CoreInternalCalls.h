#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "ScriptingCore.h"
#include "Steel/Resources/ResourceID.h"
#include "Steel/Scripting/InternalStructures/MaterialPropertyBlockInternal.h"

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

    // Resources Manager
    ResourceID ResourcesManager_LoadImage(MonoString* path);
    ResourceID ResourcesManager_LoadAsepriteData(MonoString* path, bool loopAll);
    ResourceID ResourcesManager_LoadAudioTrack(MonoString* path);
    ResourceID ResourcesManager_LoadShader(MonoString* vsPath, MonoString* fsPath);
    ResourceID ResourcesManager_CreateMaterial(ResourceID shaderID);
    float AudioTrack_GetLength(ResourceID audioTrackID);
    void Sprite_SetAsNormal(ResourceID spriteID);
    void Sprite_SetAsSpriteSheet(ResourceID spriteID, int tileWidth, int tileHeight);
    void Sprite_SetAs9Sliced(ResourceID spriteID, int offset);
    void Sprite_SetAs9Sliced2(ResourceID spriteID, int offsetTop, int offsetBottom, int offsetLeft, int offsetRight);
    uint32_t Sprite_GetTextureID(ResourceID spriteID);
    int Sprite_GetWidth(ResourceID spriteID);
    int Sprite_GetHeight(ResourceID spriteID);
    int Sprite_GetPixelsPerUnit(ResourceID spriteID);
    void Sprite_SetPixelsPerUnit(ResourceID spriteID, int pixelsPerUnit);
    void Sprite_GetPivot(ResourceID spriteID, glm::vec2* pivot);
    void Sprite_SetPivot(ResourceID spriteID, glm::vec2* pivot);
    ResourceID Animation_FromSpriteSheet(ResourceID spriteID, float length);
    ResourceID Animation_FromSprites(MonoArray* spritesIDs, float length);
    float Animation_GetLength(ResourceID animationID);
    void Animation_EndWithEmptyFrame(ResourceID animationID);
    MonoString* Animation_GetName(ResourceID animationID);
    void Animation_SetName(ResourceID animationID, MonoString* name);
    bool Animation_GetLoop(ResourceID animationID);
    void Animation_SetLoop(ResourceID animationID, bool loop);
    MonoArray* AsepriteData_GetSprites(ResourceID resourceID);
    MonoArray* AsepriteData_GetAnimations(ResourceID resourceID);
    EntityID AsepriteData_CreateEntityFromAsepriteData(ResourceID resourceID);
    ResourceID Material_GetShader(ResourceID resourceID);
    void Material_SetShader(ResourceID resourceID, ResourceID shaderID);
    void Material_GetProperties(ResourceID resourceID, MaterialPropertyBlockInternal* properties);
    void Material_SetProperties(ResourceID resourceID, MaterialPropertyBlockInternal properties);

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
