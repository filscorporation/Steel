#include "CoreInternalCalls.h"
#include "../Core/Application.h"
#include "../Core/Input.h"
#include "../Core/Log.h"
#include "../Core/Time.h"
#include "../Debug/Debug.h"
#include "../Physics/Physics.h"
#include "../Math/Random.h"
#include "../Math/PerlinNoise.h"

void CoreInternalCalls::Application_Quit()
{
    Application::Instance->Quit();
}

int CoreInternalCalls::Application_GetState()
{
    return Application::Instance->State();
}

MonoString* CoreInternalCalls::Application_RuntimePath()
{
    return mono_string_new(mono_domain_get(), Application::Instance->GetRuntimePath().c_str());
}

MonoString* CoreInternalCalls::Application_DataPath()
{
    return mono_string_new(mono_domain_get(), Application::Instance->GetDataPath().c_str());
}

void CoreInternalCalls::Input_GetMousePosition(glm::vec2* position)
{
    auto temp = Input::GetMousePosition();
    position->x = temp.x;
    position->y = temp.y;
}

void CoreInternalCalls::Input_GetMouseScrollDelta(glm::vec2* delta)
{
    auto temp = Input::GetMouseScrollDelta();
    delta->x = temp.x;
    delta->y = temp.y;
}

bool CoreInternalCalls::Input_IsKeyPressed(int code)
{
    return Input::IsKeyPressed((KeyCodes::KeyCode)code);
}

bool CoreInternalCalls::Input_IsKeyJustPressed(int code)
{
    return Input::IsKeyJustPressed((KeyCodes::KeyCode)code);
}

bool CoreInternalCalls::Input_IsKeyJustReleased(int code)
{
    return Input::IsKeyJustReleased((KeyCodes::KeyCode)code);
}

bool CoreInternalCalls::Input_IsMousePressed(int code)
{
    return Input::IsMouseButtonPressed((MouseCodes::MouseCode)code);
}

bool CoreInternalCalls::Input_IsMouseJustPressed(int code)
{
    return Input::IsMouseButtonJustPressed((MouseCodes::MouseCode)code);
}

bool CoreInternalCalls::Input_IsMouseJustReleased(int code)
{
    return Input::IsMouseButtonJustReleased((MouseCodes::MouseCode)code);
}

ResourceID CoreInternalCalls::ResourcesManager_LoadImage(MonoString* path)
{
    auto image = Application::Instance->GetResourcesManager()->LoadImage(mono_string_to_utf8(path));
    return image == nullptr ? NULL_RESOURCE : image->ID;
}

EntityID CoreInternalCalls::ResourcesManager_LoadAsepriteData(MonoString* path, bool loopAll)
{
    auto data = Application::Instance->GetResourcesManager()->LoadAsepriteData(mono_string_to_utf8(path), loopAll);
    return data == nullptr ? NULL_RESOURCE : data->ID;
}

ResourceID CoreInternalCalls::ResourcesManager_LoadAudioTrack(MonoString* path)
{
    auto track = Application::Instance->GetResourcesManager()->LoadAudioTrack(mono_string_to_utf8(path));
    return track == nullptr ? NULL_RESOURCE : track->ID;
}

float CoreInternalCalls::AudioTrack_GetLength(ResourceID audioTrackID)
{
    auto track = Application::Instance->GetResourcesManager()->GetAudioTrack(audioTrackID);
    if (track != nullptr)
        return track->LengthInSeconds;
    return 0;
}

void CoreInternalCalls::Sprite_SetAsSpriteSheet(ResourceID spriteID, int tileWidth, int tileHeight)
{
    auto image = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    if (image != nullptr)
        image->SetAsSpriteSheet(tileWidth, tileHeight);
}

int CoreInternalCalls::Sprite_GetWidth(ResourceID spriteID)
{
    auto image = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    return image == nullptr ? 0 : image->Width;
}

int CoreInternalCalls::Sprite_GetHeight(ResourceID spriteID)
{
    auto image = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    return image == nullptr ? 0 : image->Height;
}

int CoreInternalCalls::Sprite_GetPixelsPerUnit(ResourceID spriteID)
{
    auto image = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    return image == nullptr ? 0 : image->PixelsPerUnit;
}

void CoreInternalCalls::Sprite_SetPixelsPerUnit(ResourceID spriteID, int pixelsPerUnit)
{
    auto image = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    if (image != nullptr)
        image->PixelsPerUnit = pixelsPerUnit;
}

void CoreInternalCalls::Sprite_GetPivot(EntityID spriteID, glm::vec2* pivot)
{
    auto image = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    if (image != nullptr)
    {
        pivot->x = image->Pivot.x;
        pivot->y = image->Pivot.y;
    }
}

void CoreInternalCalls::Sprite_SetPivot(EntityID spriteID, glm::vec2* pivot)
{
    auto image = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    if (image != nullptr)
    {
        image->Pivot.x = pivot->x;
        image->Pivot.y = pivot->y;
    }
}

ResourceID CoreInternalCalls::Animation_FromSpriteSheet(ResourceID spriteID, float length)
{
    auto sprite = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    if (sprite == nullptr)
        return NULL_RESOURCE;

    auto animation = new Animation(sprite, length);
    Application::Instance->GetResourcesManager()->AddAnimation(animation);

    return animation->ID;
}

ResourceID CoreInternalCalls::Animation_FromSprites(MonoArray* spritesIDs, float length)
{
    std::vector<ResourceID> ids;
    ScriptingCore::FromMonoUInt32Array(spritesIDs, ids);

    std::vector<Sprite*> sprites;
    sprites.reserve(ids.size());
    for (auto id : ids)
    {
        sprites.push_back(Application::Instance->GetResourcesManager()->GetImage(id));
    }
    auto animation = new Animation(sprites, length);
    Application::Instance->GetResourcesManager()->AddAnimation(animation);

    return animation->ID;
}

float CoreInternalCalls::Animation_GetLength(ResourceID animationID)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return false;

    return animation->Length();
}

void CoreInternalCalls::Animation_EndWithEmptyFrame(ResourceID animationID)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return;

    animation->EndWithNull();
}

MonoString* CoreInternalCalls::Animation_GetName(ResourceID animationID)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return nullptr;

    return mono_string_new(mono_domain_get(), animation->Name.c_str());
}

void CoreInternalCalls::Animation_SetName(ResourceID animationID, MonoString* name)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return;

    animation->Name = mono_string_to_utf8(name);
}

bool CoreInternalCalls::Animation_GetLoop(ResourceID animationID)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return false;

    return animation->Loop;
}

void CoreInternalCalls::Animation_SetLoop(ResourceID animationID, bool loop)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
        return;

    animation->Loop = loop;
}

MonoArray* CoreInternalCalls::AsepriteData_GetSprites(ResourceID resourceID)
{
    auto data = Application::Instance->GetResourcesManager()->GetAsepriteData(resourceID);
    if (data == nullptr)
        return nullptr;

    std::vector<uint32_t> spritesIDs;
    spritesIDs.reserve(data->Sprites.size());
    for (auto sprite : data->Sprites)
    {
        spritesIDs.push_back(sprite->ID);
    }

    return ScriptingCore::ToMonoUInt32Array(spritesIDs);
}

MonoArray* CoreInternalCalls::AsepriteData_GetAnimations(ResourceID resourceID)
{
    auto data = Application::Instance->GetResourcesManager()->GetAsepriteData(resourceID);
    if (data == nullptr)
        return nullptr;

    std::vector<uint32_t> animationsIDs;
    animationsIDs.reserve(data->Animations.size());
    for (auto animation : data->Animations)
    {
        animationsIDs.push_back(animation->ID);
    }

    return ScriptingCore::ToMonoUInt32Array(animationsIDs);
}

EntityID CoreInternalCalls::AsepriteData_CreateEntityFromAsepriteData(ResourceID resourceID)
{
    auto data = Application::Instance->GetResourcesManager()->GetAsepriteData(resourceID);
    if (data == nullptr)
        return NULL_ENTITY;

    return Application::Instance->GetCurrentScene()->CreateEntity(*data);
}

float CoreInternalCalls::Time_GetDeltaTime()
{
    return Time::DeltaTime();
}

float CoreInternalCalls::Time_GetTimeScale()
{
    return Time::TimeScale;
}

void CoreInternalCalls::Time_SetTimeScale(float timeScale)
{
    Time::TimeScale = timeScale;
}

int CoreInternalCalls::Screen_GetWidth()
{
    return Screen::GetWidth();
}

void CoreInternalCalls::Screen_SetWidth(int width)
{
    Screen::SetWidth(width);
}

int CoreInternalCalls::Screen_GetHeight()
{
    return Screen::GetHeight();
}

void CoreInternalCalls::Screen_SetHeight(int height)
{
    Screen::SetHeight(height);
}

bool CoreInternalCalls::Screen_GetFullscreen()
{
    return Screen::GetFullscreen();
}

void CoreInternalCalls::Screen_SetFullscreen(bool fullscreen)
{
    Screen::SetFullscreen(fullscreen);
}

glm::vec4 CoreInternalCalls::Screen_GetColor()
{
    return glm::vec4(Screen::GetColor(), 1.0f);
}

void CoreInternalCalls::Screen_SetColor(glm::vec4 color)
{
    Screen::SetColor(color);
}

void CoreInternalCalls::Log_LogInfo(MonoString* message)
{
    Log::LogInfo(mono_string_to_utf8(message));
}

void CoreInternalCalls::Log_LogWarning(MonoString* message)
{
    Log::LogWarning(mono_string_to_utf8(message));
}

void CoreInternalCalls::Log_LogError(MonoString* message)
{
    Log::LogError(mono_string_to_utf8(message));
}

void CoreInternalCalls::Debug_EnableDebugInfoWindow()
{
    Debug::EnableDebugWindow();
}

void CoreInternalCalls::Debug_DisableDebugInfoWindow()
{
    Debug::DisableDebugWindow();
}

void CoreInternalCalls::Physics_Simulate(float deltaTime)
{
    Physics::Simulate(deltaTime);
}

MonoArray* CoreInternalCalls::Physics_PointCast(glm::vec2* point)
{
    return ScriptingCore::ToMonoUInt32Array(Physics::PointCast(*point));
}

MonoArray* CoreInternalCalls::Physics_AABBCast(glm::vec2* center, glm::vec2* size)
{
    return ScriptingCore::ToMonoUInt32Array(Physics::AABBCast(*center, *size));
}

MonoArray* CoreInternalCalls::Physics_LineCast(glm::vec2* pointA, glm::vec2* pointB)
{
    return ScriptingCore::ToMonoDataTypeArray<RayCastHit>(Physics::LineCast(*pointA, *pointB), 0);
}

float CoreInternalCalls::Random_NextFloat(float min, float max)
{
    return Random::NextFloat(min, max);
}

int CoreInternalCalls::Random_NextInt(int min, int max)
{
    return Random::NextInt(min, max);
}

float CoreInternalCalls::Random_PerlinNoise(float x, float y)
{
    return PerlinNoise::Get(x, y);
}
