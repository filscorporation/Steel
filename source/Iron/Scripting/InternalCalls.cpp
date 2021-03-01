#include "InternalCalls.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../Core/Time.h"

void InternalCalls::Application_Quit()
{
    Application::Instance->Quit();
}

int InternalCalls::Application_GetState()
{
    return Application::Instance->State();
}

MonoString* InternalCalls::Application_RuntimePath()
{
    return mono_string_new(mono_domain_get(), Application::Instance->GetRuntimePath().c_str());
}

MonoString* InternalCalls::Application_DataPath()
{
    return mono_string_new(mono_domain_get(), Application::Instance->GetDataPath().c_str());
}

glm::vec2 InternalCalls::Input_GetMousePosition()
{
    return Input::GetMousePosition();
}

glm::vec2 InternalCalls::Input_GetMouseScrollDelta()
{
    return Input::GetMouseScrollDelta();
}

bool InternalCalls::Input_IsKeyPressed(int code)
{
    return Input::IsKeyPressed((KeyCodes::KeyCode)code);
}

bool InternalCalls::Input_IsKeyJustPressed(int code)
{
    return Input::IsKeyJustPressed((KeyCodes::KeyCode)code);
}

bool InternalCalls::Input_IsKeyJustReleased(int code)
{
    return Input::IsKeyJustReleased((KeyCodes::KeyCode)code);
}

bool InternalCalls::Input_IsMousePressed(int code)
{
    return Input::IsMouseButtonPressed((MouseCodes::MouseCode)code);
}

bool InternalCalls::Input_IsMouseJustPressed(int code)
{
    return Input::IsMouseButtonJustPressed((MouseCodes::MouseCode)code);
}

bool InternalCalls::Input_IsMouseJustReleased(int code)
{
    return Input::IsMouseButtonJustReleased((MouseCodes::MouseCode)code);
}


uint64_t InternalCalls::ResourcesManager_LoadImage(MonoString* path)
{
    auto image = Application::Instance->GetResourcesManager()->LoadImage(mono_string_to_utf8(path));
    return image == nullptr ? 0 : image->ID;
}

void InternalCalls::Sprite_SetAsSpriteSheet(uint64_t spriteID, int tileWidth, int tileHeight)
{
    auto image = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    if (image != nullptr)
        image->SetAsSpriteSheet(tileWidth, tileHeight);
}

int InternalCalls::Sprite_GetWidth(uint64_t spriteID)
{
    auto image = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    return image == nullptr ? 0 : image->Width;
}

int InternalCalls::Sprite_GetHeight(uint64_t spriteID)
{
    auto image = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    return image == nullptr ? 0 : image->Height;
}

uint64_t InternalCalls::Animation_FromSpriteSheet(uint64_t spriteID, float length)
{
    auto sprite = Application::Instance->GetResourcesManager()->GetImage(spriteID);
    if (sprite == nullptr)
    {
        Log::LogError("Sprite does not exist");
        return 0;
    }

    auto animation = new Animation(sprite, length);
    Application::Instance->GetResourcesManager()->AddAnimation(animation);

    return animation->ID;
}

bool InternalCalls::Animation_GetLoop(uint64_t animationID)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
    {
        Log::LogError("Animation does not exist");
        return false;
    }

    return animation->Loop;
}

void InternalCalls::Animation_SetLoop(uint64_t animationID, bool loop)
{
    auto animation = Application::Instance->GetResourcesManager()->GetAnimation(animationID);
    if (animation == nullptr)
    {
        Log::LogError("Animation does not exist");
        return;
    }

    animation->Loop = loop;
}

float InternalCalls::Time_GetDeltaTime()
{
    return Time::DeltaTime();
}

float InternalCalls::Time_GetTimeScale()
{
    return Time::TimeScale;
}

void InternalCalls::Time_SetTimeScale(float timeScale)
{
    Time::TimeScale = timeScale;
}

int InternalCalls::Screen_GetWidth()
{
    return Screen::GetWidth();
}

void InternalCalls::Screen_SetWidth(int width)
{
    Screen::SetWidth(width);
}

int InternalCalls::Screen_GetHeight()
{
    return Screen::GetHeight();
}

void InternalCalls::Screen_SetHeight(int height)
{
    Screen::SetHeight(height);
}

bool InternalCalls::Screen_GetFullscreen()
{
    return Screen::GetFullscreen();
}

void InternalCalls::Screen_SetFullscreen(bool fullscreen)
{
    Screen::SetFullscreen(fullscreen);
}

glm::vec4 InternalCalls::Screen_GetColor()
{
    return glm::vec4(Screen::GetColor(), 1.0f);
}

void InternalCalls::Screen_SetColor(glm::vec4 color)
{
    Screen::SetColor(color);
}

void InternalCalls::Log_LogInfo(MonoString* message)
{
    Log::LogInfo(mono_string_to_utf8(message));
}

void InternalCalls::Log_LogWarning(MonoString* message)
{
    Log::LogWarning(mono_string_to_utf8(message));
}

void InternalCalls::Log_LogError(MonoString* message)
{
    Log::LogError(mono_string_to_utf8(message));
}
