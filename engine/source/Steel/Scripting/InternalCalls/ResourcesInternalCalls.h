#pragma once

#include "Steel/Scripting/ScriptingCore.h"
#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Resources/ResourceID.h"
#include "Steel/Scripting/InternalStructures/MaterialPropertyBlockInternal.h"

namespace ResourcesInternalCalls
{
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
};
