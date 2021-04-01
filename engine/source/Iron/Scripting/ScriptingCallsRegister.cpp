#include "ScriptingCallsRegister.h"
#include "ComponentsInternalCalls.h"
#include "EntityInternalCalls.h"
#include "CoreInternalCalls.h"
#include "UIInternalCalls.h"

void ScriptingCallsRegister::RegisterInternalCalls()
{
    mono_add_internal_call("Iron.Application::Quit_Internal", (void*)CoreInternalCalls::Application_Quit);
    mono_add_internal_call("Iron.Application::GetState_Internal", (void*)CoreInternalCalls::Application_GetState);
    mono_add_internal_call("Iron.Application::GetRuntimePath_Internal", (void*)CoreInternalCalls::Application_RuntimePath);
    mono_add_internal_call("Iron.Application::GetDataPath_Internal", (void*)CoreInternalCalls::Application_DataPath);
    mono_add_internal_call("Iron.Log::LogInfo_Internal", (void*)CoreInternalCalls::Log_LogInfo);
    mono_add_internal_call("Iron.Log::LogWarning_Internal", (void*)CoreInternalCalls::Log_LogWarning);
    mono_add_internal_call("Iron.Log::LogError_Internal", (void*)CoreInternalCalls::Log_LogError);
    mono_add_internal_call("Iron.Debug::EnableDebugInfoWindow_Internal", (void*)CoreInternalCalls::Debug_EnableDebugInfoWindow);
    mono_add_internal_call("Iron.Debug::DisableDebugInfoWindow_Internal", (void*)CoreInternalCalls::Debug_DisableDebugInfoWindow);
    mono_add_internal_call("Iron.Time::GetDeltaTime_Internal", (void*)CoreInternalCalls::Time_GetDeltaTime);
    mono_add_internal_call("Iron.Time::GetTimeScale_Internal", (void*)CoreInternalCalls::Time_GetTimeScale);
    mono_add_internal_call("Iron.Time::SetTimeScale_Internal", (void*)CoreInternalCalls::Time_SetTimeScale);

    mono_add_internal_call("Iron.Input::GetMousePosition_Internal", (void*)CoreInternalCalls::Input_GetMousePosition);
    mono_add_internal_call("Iron.Input::GetMouseScrollDelta_Internal", (void*)CoreInternalCalls::Input_GetMouseScrollDelta);
    mono_add_internal_call("Iron.Input::IsKeyPressed_Internal", (void*)CoreInternalCalls::Input_IsKeyPressed);
    mono_add_internal_call("Iron.Input::IsKeyJustPressed_Internal", (void*)CoreInternalCalls::Input_IsKeyJustPressed);
    mono_add_internal_call("Iron.Input::IsKeyJustReleased_Internal", (void*)CoreInternalCalls::Input_IsKeyJustReleased);
    mono_add_internal_call("Iron.Input::IsMousePressed_Internal", (void*)CoreInternalCalls::Input_IsMousePressed);
    mono_add_internal_call("Iron.Input::IsMouseJustPressed_Internal", (void*)CoreInternalCalls::Input_IsMouseJustPressed);
    mono_add_internal_call("Iron.Input::IsMouseJustReleased_Internal", (void*)CoreInternalCalls::Input_IsMouseJustReleased);

    mono_add_internal_call("Iron.Screen::GetWidth_Internal", (void*)CoreInternalCalls::Screen_GetWidth);
    mono_add_internal_call("Iron.Screen::SetWidth_Internal", (void*)CoreInternalCalls::Screen_SetWidth);
    mono_add_internal_call("Iron.Screen::GetHeight_Internal", (void*)CoreInternalCalls::Screen_GetHeight);
    mono_add_internal_call("Iron.Screen::SetHeight_Internal", (void*)CoreInternalCalls::Screen_SetHeight);
    mono_add_internal_call("Iron.Screen::GetFullscreen_Internal", (void*)CoreInternalCalls::Screen_GetFullscreen);
    mono_add_internal_call("Iron.Screen::SetFullscreen_Internal", (void*)CoreInternalCalls::Screen_SetFullscreen);
    mono_add_internal_call("Iron.Screen::GetColor_Internal", (void*)CoreInternalCalls::Screen_GetColor);
    mono_add_internal_call("Iron.Screen::SetColor_Internal", (void*)CoreInternalCalls::Screen_SetColor);

    mono_add_internal_call("Iron.Physics::Simulate_Internal", (void*)CoreInternalCalls::Physics_Simulate);
    mono_add_internal_call("Iron.Physics::PointCast_Internal", (void*)CoreInternalCalls::Physics_PointCast);
    mono_add_internal_call("Iron.Physics::AABBCast_Internal", (void*)CoreInternalCalls::Physics_AABBCast);
    mono_add_internal_call("Iron.Physics::LineCast_Internal", (void*)CoreInternalCalls::Physics_LineCast);

    mono_add_internal_call("Iron.Random::NextFloat_Internal", (void*)CoreInternalCalls::Random_NextFloat);
    mono_add_internal_call("Iron.Random::NextInt_Internal", (void*)CoreInternalCalls::Random_NextInt);
    mono_add_internal_call("Iron.Random::PerlinNoise_Internal", (void*)CoreInternalCalls::Random_PerlinNoise);

    mono_add_internal_call("Iron.ResourcesManager::LoadImage_Internal", (void*)CoreInternalCalls::ResourcesManager_LoadImage);
    mono_add_internal_call("Iron.ResourcesManager::LoadAudioTrack_Internal", (void*)CoreInternalCalls::ResourcesManager_LoadAudioTrack);
    mono_add_internal_call("Iron.AudioTrack::GetLength_Internal", (void*)CoreInternalCalls::AudioTrack_GetLength);
    mono_add_internal_call("Iron.Sprite::SetAsSpriteSheet_Internal", (void*)CoreInternalCalls::Sprite_SetAsSpriteSheet);
    mono_add_internal_call("Iron.Sprite::GetWidth_Internal", (void*)CoreInternalCalls::Sprite_GetWidth);
    mono_add_internal_call("Iron.Sprite::GetHeight_Internal", (void*)CoreInternalCalls::Sprite_GetHeight);
    mono_add_internal_call("Iron.Animation::FromSpriteSheet_Internal", (void*)CoreInternalCalls::Animation_FromSpriteSheet);
    mono_add_internal_call("Iron.Animation::GetLoop_Internal", (void*)CoreInternalCalls::Animation_GetLoop);
    mono_add_internal_call("Iron.Animation::GetLength_Internal", (void*)CoreInternalCalls::Animation_GetLength);
    mono_add_internal_call("Iron.Animation::SetLoop_Internal", (void*)CoreInternalCalls::Animation_SetLoop);

    mono_add_internal_call("Iron.Entity::CreateNewEntity_Internal", (void*)EntityInternalCalls::Entity_CreateNewEntity);
    mono_add_internal_call("Iron.Entity::CreateNewEntity_Internal2", (void*)EntityInternalCalls::Entity_CreateNewEntity2);
    mono_add_internal_call("Iron.Entity::CreateNewEntity_Internal3", (void*)EntityInternalCalls::Entity_CreateNewEntity3);
    mono_add_internal_call("Iron.Entity::DestroyEntity_Internal", (void*)EntityInternalCalls::Entity_DestroyEntity);
    mono_add_internal_call("Iron.Entity::AddComponent_Internal", (void*)EntityInternalCalls::Entity_AddComponent);
    mono_add_internal_call("Iron.Entity::AddScriptComponent_Internal", (void*)EntityInternalCalls::Entity_AddScriptComponent);
    mono_add_internal_call("Iron.Entity::HasComponent_Internal", (void *) EntityInternalCalls::Entity_HasComponent);
    mono_add_internal_call("Iron.Entity::GetScriptComponent_Internal", (void*)EntityInternalCalls::Entity_GetScriptComponent);
    mono_add_internal_call("Iron.Entity::RemoveComponent_Internal", (void*)EntityInternalCalls::Entity_RemoveComponent);
    mono_add_internal_call("Iron.Entity::RemoveScriptComponent_Internal", (void*)EntityInternalCalls::Entity_RemoveScriptComponent);
    mono_add_internal_call("Iron.Entity::GetName_Internal", (void*)EntityInternalCalls::Entity_GetName);
    mono_add_internal_call("Iron.Entity::SetName_Internal", (void*)EntityInternalCalls::Entity_SetName);
    mono_add_internal_call("Iron.Entity::GetParent_Internal", (void*)ComponentsInternalCalls::HierarchyNode_GetParent);
    mono_add_internal_call("Iron.Entity::SetParent_Internal", (void*)ComponentsInternalCalls::HierarchyNode_SetParent);
    mono_add_internal_call("Iron.Entity::GetChildren_Internal", (void*)ComponentsInternalCalls::HierarchyNode_GetChildren);
    mono_add_internal_call("Iron.Entity::GetIsActive_Internal", (void*)EntityInternalCalls::Entity_GetIsActive);
    mono_add_internal_call("Iron.Entity::GetIsActiveSelf_Internal", (void*)EntityInternalCalls::Entity_GetIsActiveSelf);
    mono_add_internal_call("Iron.Entity::SetIsActiveSelf_Internal", (void*)EntityInternalCalls::Entity_SetIsActiveSelf);
    mono_add_internal_call("Iron.Entity::IsDestroyed_Internal", (void*)EntityInternalCalls::Entity_IsDestroyed);

    mono_add_internal_call("Iron.Component::FindAllOfType_Internal", (void*)EntityInternalCalls::Component_FindAllOfType);
    mono_add_internal_call("Iron.Component::FindAllScriptsOfType_Internal", (void*)EntityInternalCalls::Component_FindAllScriptsOfType);

    mono_add_internal_call("Iron.Transformation::GetPosition_Internal", (void*)ComponentsInternalCalls::Transformation_GetPosition);
    mono_add_internal_call("Iron.Transformation::SetPosition_Internal", (void*)ComponentsInternalCalls::Transformation_SetPosition);
    mono_add_internal_call("Iron.Transformation::GetRotation_Internal", (void*)ComponentsInternalCalls::Transformation_GetRotation);
    mono_add_internal_call("Iron.Transformation::SetRotation_Internal", (void*)ComponentsInternalCalls::Transformation_SetRotation);
    mono_add_internal_call("Iron.Transformation::GetScale_Internal", (void*)ComponentsInternalCalls::Transformation_GetScale);
    mono_add_internal_call("Iron.Transformation::SetScale_Internal", (void*)ComponentsInternalCalls::Transformation_SetScale);

    mono_add_internal_call("Iron.AudioListener::GetVolume_Internal", (void*)ComponentsInternalCalls::AudioListener_GetVolume);
    mono_add_internal_call("Iron.AudioListener::SetVolume_Internal", (void*)ComponentsInternalCalls::AudioListener_SetVolume);
    mono_add_internal_call("Iron.AudioSource::GetVolume_Internal", (void*)ComponentsInternalCalls::AudioSource_GetVolume);
    mono_add_internal_call("Iron.AudioSource::SetVolume_Internal", (void*)ComponentsInternalCalls::AudioSource_SetVolume);
    mono_add_internal_call("Iron.AudioSource::GetLoop_Internal", (void*)ComponentsInternalCalls::AudioSource_GetLoop);
    mono_add_internal_call("Iron.AudioSource::SetLoop_Internal", (void*)ComponentsInternalCalls::AudioSource_SetLoop);
    mono_add_internal_call("Iron.AudioSource::Play_Internal", (void*)ComponentsInternalCalls::AudioSource_Play);
    mono_add_internal_call("Iron.AudioSource::Stop_Internal", (void*)ComponentsInternalCalls::AudioSource_Stop);

    mono_add_internal_call("Iron.SpriteRenderer::GetSprite_Internal", (void*)ComponentsInternalCalls::SpriteRenderer_GetSprite);
    mono_add_internal_call("Iron.SpriteRenderer::SetSprite_Internal", (void*)ComponentsInternalCalls::SpriteRenderer_SetSprite);

    mono_add_internal_call("Iron.Animator::PlayAnimation_Internal", (void*)ComponentsInternalCalls::Animator_PlayAnimation);
    mono_add_internal_call("Iron.Animator::Play_Internal", (void*)ComponentsInternalCalls::Animator_Play);
    mono_add_internal_call("Iron.Animator::Pause_Internal", (void*)ComponentsInternalCalls::Animator_Pause);
    mono_add_internal_call("Iron.Animator::Stop_Internal", (void*)ComponentsInternalCalls::Animator_Stop);
    mono_add_internal_call("Iron.Animator::Restart_Internal", (void*)ComponentsInternalCalls::Animator_Restart);

    mono_add_internal_call("Iron.Camera::GetEntityWithMainCamera_Internal", (void *) ComponentsInternalCalls::Camera_GetEntityWithMainCamera);
    mono_add_internal_call("Iron.Camera::GetWidth_Internal", (void*)ComponentsInternalCalls::Camera_GetWidth);
    mono_add_internal_call("Iron.Camera::SetWidth_Internal", (void*)ComponentsInternalCalls::Camera_SetWidth);
    mono_add_internal_call("Iron.Camera::GetHeight_Internal", (void*)ComponentsInternalCalls::Camera_GetHeight);
    mono_add_internal_call("Iron.Camera::SetHeight_Internal", (void*)ComponentsInternalCalls::Camera_SetHeight);
    mono_add_internal_call("Iron.Camera::GetNearClippingPlane_Internal", (void*)ComponentsInternalCalls::Camera_GetNearClippingPlane);
    mono_add_internal_call("Iron.Camera::SetNearClippingPlane_Internal", (void*)ComponentsInternalCalls::Camera_SetNearClippingPlane);
    mono_add_internal_call("Iron.Camera::GetFarClippingPlane_Internal", (void*)ComponentsInternalCalls::Camera_GetFarClippingPlane);
    mono_add_internal_call("Iron.Camera::SetFarClippingPlane_Internal", (void*)ComponentsInternalCalls::Camera_SetFarClippingPlane);
    mono_add_internal_call("Iron.Camera::GetResizingMode_Internal", (void*)ComponentsInternalCalls::Camera_GetResizingMode);
    mono_add_internal_call("Iron.Camera::SetResizingMode_Internal", (void*)ComponentsInternalCalls::Camera_SetResizingMode);
    mono_add_internal_call("Iron.Camera::WorldToScreenPoint_Internal", (void*)ComponentsInternalCalls::Camera_WorldToScreenPoint);
    mono_add_internal_call("Iron.Camera::ScreenToWorldPoint_Internal", (void*)ComponentsInternalCalls::Camera_ScreenToWorldPoint);

    mono_add_internal_call("Iron.RigidBody::GetMass_Internal", (void*)ComponentsInternalCalls::RigidBody_GetMass);
    mono_add_internal_call("Iron.RigidBody::SetMass_Internal", (void*)ComponentsInternalCalls::RigidBody_SetMass);
    mono_add_internal_call("Iron.RigidBody::GetRigidBodyType_Internal", (void*)ComponentsInternalCalls::RigidBody_GetRigidBodyType);
    mono_add_internal_call("Iron.RigidBody::SetRigidBodyType_Internal", (void*)ComponentsInternalCalls::RigidBody_SetRigidBodyType);
    mono_add_internal_call("Iron.BoxCollider::GetSize_Internal", (void*)ComponentsInternalCalls::BoxCollider_GetSize);
    mono_add_internal_call("Iron.BoxCollider::SetSize_Internal", (void*)ComponentsInternalCalls::BoxCollider_SetSize);
    mono_add_internal_call("Iron.CircleCollider::GetRadius_Internal", (void*)ComponentsInternalCalls::CircleCollider_GetRadius);
    mono_add_internal_call("Iron.CircleCollider::SetRadius_Internal", (void*)ComponentsInternalCalls::CircleCollider_SetRadius);

    mono_add_internal_call("Iron.RectTransformation::GetAnchorMin_Internal", (void*)UIInternalCalls::RectTransformation_GetAnchorMin);
    mono_add_internal_call("Iron.RectTransformation::SetAnchorMin_Internal", (void*)UIInternalCalls::RectTransformation_SetAnchorMin);
    mono_add_internal_call("Iron.RectTransformation::GetAnchorMax_Internal", (void*)UIInternalCalls::RectTransformation_GetAnchorMax);
    mono_add_internal_call("Iron.RectTransformation::SetAnchorMax_Internal", (void*)UIInternalCalls::RectTransformation_SetAnchorMax);
    mono_add_internal_call("Iron.RectTransformation::GetAnchoredPosition_Internal", (void*)UIInternalCalls::RectTransformation_GetAnchoredPosition);
    mono_add_internal_call("Iron.RectTransformation::SetAnchoredPosition_Internal", (void*)UIInternalCalls::RectTransformation_SetAnchoredPosition);
    mono_add_internal_call("Iron.RectTransformation::GetOffsetMin_Internal", (void*)UIInternalCalls::RectTransformation_GetOffsetMin);
    mono_add_internal_call("Iron.RectTransformation::SetOffsetMin_Internal", (void*)UIInternalCalls::RectTransformation_SetOffsetMin);
    mono_add_internal_call("Iron.RectTransformation::GetOffsetMax_Internal", (void*)UIInternalCalls::RectTransformation_GetOffsetMax);
    mono_add_internal_call("Iron.RectTransformation::SetOffsetMax_Internal", (void*)UIInternalCalls::RectTransformation_SetOffsetMax);
    mono_add_internal_call("Iron.RectTransformation::GetPivot_Internal", (void*)UIInternalCalls::RectTransformation_GetPivot);
    mono_add_internal_call("Iron.RectTransformation::SetPivot_Internal", (void*)UIInternalCalls::RectTransformation_SetPivot);
    mono_add_internal_call("Iron.RectTransformation::GetSize_Internal", (void*)UIInternalCalls::RectTransformation_GetSize);
    mono_add_internal_call("Iron.RectTransformation::SetSize_Internal", (void*)UIInternalCalls::RectTransformation_SetSize);
    mono_add_internal_call("Iron.RectTransformation::GetSortingOrder_Internal", (void*)UIInternalCalls::RectTransformation_GetSortingOrder);
    mono_add_internal_call("Iron.RectTransformation::SetSortingOrder_Internal", (void*)UIInternalCalls::RectTransformation_SetSortingOrder);
    mono_add_internal_call("Iron.RectTransformation::GetRotation_Internal", (void*)UIInternalCalls::RectTransformation_GetRotation);
    mono_add_internal_call("Iron.RectTransformation::SetRotation_Internal", (void*)UIInternalCalls::RectTransformation_SetRotation);

    mono_add_internal_call("Iron.UIImage::GetSprite_Internal", (void*)UIInternalCalls::UIImage_GetSprite);
    mono_add_internal_call("Iron.UIImage::SetSprite_Internal", (void*)UIInternalCalls::UIImage_SetSprite);

    mono_add_internal_call("Iron.UIButton::GetSprite_Internal", (void*)UIInternalCalls::UIButton_GetSprite);
    mono_add_internal_call("Iron.UIButton::SetSprite_Internal", (void*)UIInternalCalls::UIButton_SetSprite);
    mono_add_internal_call("Iron.UIButton::GetInteractable_Internal", (void*)UIInternalCalls::UIButton_GetInteractable);
    mono_add_internal_call("Iron.UIButton::SetInteractable_Internal", (void*)UIInternalCalls::UIButton_SetInteractable);
    mono_add_internal_call("Iron.UIButton::GetTransition_Internal", (void*)UIInternalCalls::UIButton_GetTransition);
    mono_add_internal_call("Iron.UIButton::SetTransition_Internal", (void*)UIInternalCalls::UIButton_SetTransition);

    mono_add_internal_call("Iron.UIText::GetText_Internal", (void*)UIInternalCalls::UIText_GetText);
    mono_add_internal_call("Iron.UIText::SetText_Internal", (void*)UIInternalCalls::UIText_SetText);
    mono_add_internal_call("Iron.UIText::GetTextSize_Internal", (void*)UIInternalCalls::UIText_GetTextSize);
    mono_add_internal_call("Iron.UIText::SetTextSize_Internal", (void*)UIInternalCalls::UIText_SetTextSize);
    mono_add_internal_call("Iron.UIText::GetColor_Internal", (void*)UIInternalCalls::UIText_GetColor);
    mono_add_internal_call("Iron.UIText::SetColor_Internal", (void*)UIInternalCalls::UIText_SetColor);
    mono_add_internal_call("Iron.UIText::GetIsTextAutoSize_Internal", (void*)UIInternalCalls::UIButton_GetIsTextAutoSize);
    mono_add_internal_call("Iron.UIText::SetIsTextAutoSize_Internal", (void*)UIInternalCalls::UIButton_SetIsTextAutoSize);
    mono_add_internal_call("Iron.UIText::GetTextAlignment_Internal", (void*)UIInternalCalls::UIButton_GetTextAlignment);
    mono_add_internal_call("Iron.UIText::SetTextAlignment_Internal", (void*)UIInternalCalls::UIButton_SetTextAlignment);

    mono_add_internal_call("Iron.UI::CreateUIElement_Internal", (void*)UIInternalCalls::UI_CreateUIElement);
    mono_add_internal_call("Iron.UI::CreateUIElement_Internal2", (void*)UIInternalCalls::UI_CreateUIElement2);
    mono_add_internal_call("Iron.UI::CreateUIElement_Internal3", (void*)UIInternalCalls::UI_CreateUIElement3);
    mono_add_internal_call("Iron.UI::IsPointerOverUI_Internal", (void*)UIInternalCalls::UI_IsPointerOverUI);
}
