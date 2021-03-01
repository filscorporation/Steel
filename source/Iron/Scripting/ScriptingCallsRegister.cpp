#include "ScriptingCallsRegister.h"
#include "InternalCalls.h"
#include "EntityInternalCalls.h"
#include "ComponentsInternalCalls.h"

void ScriptingCallsRegister::RegisterInternalCalls()
{
    mono_add_internal_call("Iron.Application::Quit_Internal", (void*)InternalCalls::Application_Quit);
    mono_add_internal_call("Iron.Application::GetState_Internal", (void*)InternalCalls::Application_GetState);
    mono_add_internal_call("Iron.Application::GetRuntimePath_Internal", (void*)InternalCalls::Application_RuntimePath);
    mono_add_internal_call("Iron.Application::GetDataPath_Internal", (void*)InternalCalls::Application_DataPath);
    mono_add_internal_call("Iron.Input::IsKeyPressed_Internal", (void*)InternalCalls::Input_IsKeyPressed);
    mono_add_internal_call("Iron.Log::LogInfo_Internal", (void*)InternalCalls::Log_LogInfo);
    mono_add_internal_call("Iron.Log::LogWarning_Internal", (void*)InternalCalls::Log_LogWarning);
    mono_add_internal_call("Iron.Log::LogError_Internal", (void*)InternalCalls::Log_LogError);
    mono_add_internal_call("Iron.Time::GetDeltaTime_Internal", (void*)InternalCalls::Time_GetDeltaTime);
    mono_add_internal_call("Iron.Time::GetTimeScale_Internal", (void*)InternalCalls::Time_GetTimeScale);
    mono_add_internal_call("Iron.Time::SetTimeScale_Internal", (void*)InternalCalls::Time_SetTimeScale);

    mono_add_internal_call("Iron.Screen::GetWidth_Internal", (void*)InternalCalls::Screen_GetWidth);
    mono_add_internal_call("Iron.Screen::SetWidth_Internal", (void*)InternalCalls::Screen_SetWidth);
    mono_add_internal_call("Iron.Screen::GetHeight_Internal", (void*)InternalCalls::Screen_GetHeight);
    mono_add_internal_call("Iron.Screen::SetHeight_Internal", (void*)InternalCalls::Screen_SetHeight);
    mono_add_internal_call("Iron.Screen::GetFullscreen_Internal", (void*)InternalCalls::Screen_GetFullscreen);
    mono_add_internal_call("Iron.Screen::SetFullscreen_Internal", (void*)InternalCalls::Screen_SetFullscreen);
    mono_add_internal_call("Iron.Screen::GetColor_Internal", (void*)InternalCalls::Screen_GetColor);
    mono_add_internal_call("Iron.Screen::SetColor_Internal", (void*)InternalCalls::Screen_SetColor);

    mono_add_internal_call("Iron.ResourcesManager::LoadImage_Internal", (void*)InternalCalls::ResourcesManager_LoadImage);
    mono_add_internal_call("Iron.Sprite::SetAsSpriteSheet_Internal", (void*)InternalCalls::Sprite_SetAsSpriteSheet);
    mono_add_internal_call("Iron.Sprite::GetWidth_Internal", (void*)InternalCalls::Sprite_GetWidth);
    mono_add_internal_call("Iron.Sprite::GetHeight_Internal", (void*)InternalCalls::Sprite_GetHeight);
    mono_add_internal_call("Iron.Animation::FromSpriteSheet_Internal", (void*)InternalCalls::Animation_FromSpriteSheet);
    mono_add_internal_call("Iron.Animation::GetLoop_Internal", (void*)InternalCalls::Animation_GetLoop);
    mono_add_internal_call("Iron.Animation::SetLoop_Internal", (void*)InternalCalls::Animation_SetLoop);

    mono_add_internal_call("Iron.Entity::CreateNewEntity_Internal", (void*)EntityInternalCalls::Entity_CreateNewEntity);
    mono_add_internal_call("Iron.Entity::AddComponent_Internal", (void*)EntityInternalCalls::Entity_AddComponent);
    mono_add_internal_call("Iron.Entity::AddScriptComponent_Internal", (void*)EntityInternalCalls::Entity_AddScriptComponent);
    mono_add_internal_call("Iron.Entity::GetComponent_Internal", (void*)EntityInternalCalls::Entity_GetComponent);
    mono_add_internal_call("Iron.Entity::GetScriptComponent_Internal", (void*)EntityInternalCalls::Entity_GetScriptComponent);
    mono_add_internal_call("Iron.Entity::RemoveComponent_Internal", (void*)EntityInternalCalls::Entity_RemoveComponent);
    mono_add_internal_call("Iron.Entity::RemoveScriptComponent_Internal", (void*)EntityInternalCalls::Entity_RemoveScriptComponent);
    mono_add_internal_call("Iron.Entity::GetName_Internal", (void*)EntityInternalCalls::Entity_GetName);
    mono_add_internal_call("Iron.Entity::SetName_Internal", (void*)EntityInternalCalls::Entity_SetName);

    mono_add_internal_call("Iron.Transformation::GetPosition_Internal", (void*)ComponentsInternalCalls::Transformation_GetPosition);
    mono_add_internal_call("Iron.Transformation::SetPosition_Internal", (void*)ComponentsInternalCalls::Transformation_SetPosition);
    mono_add_internal_call("Iron.Transformation::GetRotation_Internal", (void*)ComponentsInternalCalls::Transformation_GetRotation);
    mono_add_internal_call("Iron.Transformation::SetRotation_Internal", (void*)ComponentsInternalCalls::Transformation_SetRotation);
    mono_add_internal_call("Iron.Transformation::GetScale_Internal", (void*)ComponentsInternalCalls::Transformation_GetScale);
    mono_add_internal_call("Iron.Transformation::SetScale_Internal", (void*)ComponentsInternalCalls::Transformation_SetScale);

    mono_add_internal_call("Iron.SpriteRenderer::GetSprite_Internal", (void*)ComponentsInternalCalls::SpriteRenderer_GetSprite);
    mono_add_internal_call("Iron.SpriteRenderer::SetSprite_Internal", (void*)ComponentsInternalCalls::SpriteRenderer_SetSprite);

    mono_add_internal_call("Iron.Animator::PlayAnimation_Internal", (void*)ComponentsInternalCalls::Animator_PlayAnimation);
    mono_add_internal_call("Iron.Animator::Play_Internal", (void*)ComponentsInternalCalls::Animator_Play);
    mono_add_internal_call("Iron.Animator::Pause_Internal", (void*)ComponentsInternalCalls::Animator_Pause);
    mono_add_internal_call("Iron.Animator::Stop_Internal", (void*)ComponentsInternalCalls::Animator_Stop);
    mono_add_internal_call("Iron.Animator::Restart_Internal", (void*)ComponentsInternalCalls::Animator_Restart);
}
