#include "ScriptingCallsRegister.h"
#include "ComponentsInternalCalls.h"
#include "EntityInternalCalls.h"
#include "CoreInternalCalls.h"
#include "UIInternalCalls.h"

void ScriptingCallsRegister::RegisterInternalCalls()
{
    mono_add_internal_call("Steel.Application::Quit_Internal", (void*)CoreInternalCalls::Application_Quit);
    mono_add_internal_call("Steel.Application::GetRuntimePath_Internal", (void*)CoreInternalCalls::Application_RuntimePath);
    mono_add_internal_call("Steel.Application::GetDataPath_Internal", (void*)CoreInternalCalls::Application_DataPath);
    mono_add_internal_call("Steel.Log::LogDebug_Internal", (void*)CoreInternalCalls::Log_LogDebug);
    mono_add_internal_call("Steel.Log::LogInfo_Internal", (void*)CoreInternalCalls::Log_LogInfo);
    mono_add_internal_call("Steel.Log::LogWarning_Internal", (void*)CoreInternalCalls::Log_LogWarning);
    mono_add_internal_call("Steel.Log::LogError_Internal", (void*)CoreInternalCalls::Log_LogError);
    mono_add_internal_call("Steel.Time::GetDeltaTime_Internal", (void*)CoreInternalCalls::Time_GetDeltaTime);
    mono_add_internal_call("Steel.Time::GetUnscaledDeltaTime_Internal", (void*)CoreInternalCalls::Time_GetUnscaledDeltaTime);
    mono_add_internal_call("Steel.Time::GetTimeSinceStartup_Internal", (void*)CoreInternalCalls::Time_GetTimeSinceStartup);
    mono_add_internal_call("Steel.Time::GetFrameCount_Internal", (void*)CoreInternalCalls::Time_GetFrameCount);
    mono_add_internal_call("Steel.Time::GetFixedDeltaTime_Internal", (void*)CoreInternalCalls::Time_GetFixedDeltaTime);
    mono_add_internal_call("Steel.Time::SetFixedDeltaTime_Internal", (void*)CoreInternalCalls::Time_SetFixedDeltaTime);
    mono_add_internal_call("Steel.Time::GetTimeScale_Internal", (void*)CoreInternalCalls::Time_GetTimeScale);
    mono_add_internal_call("Steel.Time::SetTimeScale_Internal", (void*)CoreInternalCalls::Time_SetTimeScale);

    mono_add_internal_call("Steel.Input::GetMousePosition_Internal", (void*)CoreInternalCalls::Input_GetMousePosition);
    mono_add_internal_call("Steel.Input::GetMouseScrollDelta_Internal", (void*)CoreInternalCalls::Input_GetMouseScrollDelta);
    mono_add_internal_call("Steel.Input::IsKeyPressed_Internal", (void*)CoreInternalCalls::Input_IsKeyPressed);
    mono_add_internal_call("Steel.Input::IsKeyJustPressed_Internal", (void*)CoreInternalCalls::Input_IsKeyJustPressed);
    mono_add_internal_call("Steel.Input::IsKeyJustReleased_Internal", (void*)CoreInternalCalls::Input_IsKeyJustReleased);
    mono_add_internal_call("Steel.Input::IsMousePressed_Internal", (void*)CoreInternalCalls::Input_IsMousePressed);
    mono_add_internal_call("Steel.Input::IsMouseJustPressed_Internal", (void*)CoreInternalCalls::Input_IsMouseJustPressed);
    mono_add_internal_call("Steel.Input::IsMouseJustReleased_Internal", (void*)CoreInternalCalls::Input_IsMouseJustReleased);

    mono_add_internal_call("Steel.Screen::GetWidth_Internal", (void*)CoreInternalCalls::Screen_GetWidth);
    mono_add_internal_call("Steel.Screen::SetWidth_Internal", (void*)CoreInternalCalls::Screen_SetWidth);
    mono_add_internal_call("Steel.Screen::GetHeight_Internal", (void*)CoreInternalCalls::Screen_GetHeight);
    mono_add_internal_call("Steel.Screen::SetHeight_Internal", (void*)CoreInternalCalls::Screen_SetHeight);
    mono_add_internal_call("Steel.Screen::GetFullscreen_Internal", (void*)CoreInternalCalls::Screen_GetFullscreen);
    mono_add_internal_call("Steel.Screen::SetFullscreen_Internal", (void*)CoreInternalCalls::Screen_SetFullscreen);
    mono_add_internal_call("Steel.Screen::GetIsMinimized_Internal", (void*)CoreInternalCalls::Screen_GetIsMinimized);
    mono_add_internal_call("Steel.Screen::GetColor_Internal", (void*)CoreInternalCalls::Screen_GetColor);
    mono_add_internal_call("Steel.Screen::SetColor_Internal", (void*)CoreInternalCalls::Screen_SetColor);

    mono_add_internal_call("Steel.Physics::Simulate_Internal", (void*)CoreInternalCalls::Physics_Simulate);
    mono_add_internal_call("Steel.Physics::PointCast_Internal", (void*)CoreInternalCalls::Physics_PointCast);
    mono_add_internal_call("Steel.Physics::AABBCast_Internal", (void*)CoreInternalCalls::Physics_AABBCast);
    mono_add_internal_call("Steel.Physics::LineCast_Internal", (void*)CoreInternalCalls::Physics_LineCast);

    mono_add_internal_call("Steel.Random::NextFloat_Internal", (void*)CoreInternalCalls::Random_NextFloat);
    mono_add_internal_call("Steel.Random::NextInt_Internal", (void*)CoreInternalCalls::Random_NextInt);
    mono_add_internal_call("Steel.Random::PerlinNoise_Internal", (void*)CoreInternalCalls::Random_PerlinNoise);

    mono_add_internal_call("Steel.ResourcesManager::LoadImage_Internal", (void*)CoreInternalCalls::ResourcesManager_LoadImage);
    mono_add_internal_call("Steel.ResourcesManager::LoadAsepriteData_Internal", (void*)CoreInternalCalls::ResourcesManager_LoadAsepriteData);
    mono_add_internal_call("Steel.ResourcesManager::LoadAudioTrack_Internal", (void*)CoreInternalCalls::ResourcesManager_LoadAudioTrack);
    mono_add_internal_call("Steel.ResourcesManager::LoadShader_Internal", (void*)CoreInternalCalls::ResourcesManager_LoadShader);
    mono_add_internal_call("Steel.ResourcesManager::CreateMaterial_Internal", (void*)CoreInternalCalls::ResourcesManager_CreateMaterial);
    mono_add_internal_call("Steel.AudioTrack::GetLength_Internal", (void*)CoreInternalCalls::AudioTrack_GetLength);
    mono_add_internal_call("Steel.Sprite::SetAsNormal_Internal", (void*)CoreInternalCalls::Sprite_SetAsNormal);
    mono_add_internal_call("Steel.Sprite::SetAsSpriteSheet_Internal", (void*)CoreInternalCalls::Sprite_SetAsSpriteSheet);
    mono_add_internal_call("Steel.Sprite::SetAs9Sliced_Internal", (void*)CoreInternalCalls::Sprite_SetAs9Sliced);
    mono_add_internal_call("Steel.Sprite::SetAs9Sliced_Internal2", (void*)CoreInternalCalls::Sprite_SetAs9Sliced2);
    mono_add_internal_call("Steel.Sprite::GetTextureID_Internal", (void*)CoreInternalCalls::Sprite_GetTextureID);
    mono_add_internal_call("Steel.Sprite::GetWidth_Internal", (void*)CoreInternalCalls::Sprite_GetWidth);
    mono_add_internal_call("Steel.Sprite::GetHeight_Internal", (void*)CoreInternalCalls::Sprite_GetHeight);
    mono_add_internal_call("Steel.Sprite::GetPixelsPerUnit_Internal", (void*)CoreInternalCalls::Sprite_GetPixelsPerUnit);
    mono_add_internal_call("Steel.Sprite::SetPixelsPerUnit_Internal", (void*)CoreInternalCalls::Sprite_SetPixelsPerUnit);
    mono_add_internal_call("Steel.Sprite::GetPivot_Internal", (void*)CoreInternalCalls::Sprite_GetPivot);
    mono_add_internal_call("Steel.Sprite::SetPivot_Internal", (void*)CoreInternalCalls::Sprite_SetPivot);
    mono_add_internal_call("Steel.Animation::FromSpriteSheet_Internal", (void*)CoreInternalCalls::Animation_FromSpriteSheet);
    mono_add_internal_call("Steel.Animation::FromSprites_Internal", (void*)CoreInternalCalls::Animation_FromSprites);
    mono_add_internal_call("Steel.Animation::GetName_Internal", (void*)CoreInternalCalls::Animation_GetName);
    mono_add_internal_call("Steel.Animation::SetName_Internal", (void*)CoreInternalCalls::Animation_SetName);
    mono_add_internal_call("Steel.Animation::GetLoop_Internal", (void*)CoreInternalCalls::Animation_GetLoop);
    mono_add_internal_call("Steel.Animation::SetLoop_Internal", (void*)CoreInternalCalls::Animation_SetLoop);
    mono_add_internal_call("Steel.Animation::GetLength_Internal", (void*)CoreInternalCalls::Animation_GetLength);
    mono_add_internal_call("Steel.Animation::EndWithEmptyFrame_Internal", (void*)CoreInternalCalls::Animation_EndWithEmptyFrame);
    mono_add_internal_call("Steel.AsepriteData::GetSprites_Internal", (void*)CoreInternalCalls::AsepriteData_GetSprites);
    mono_add_internal_call("Steel.AsepriteData::GetAnimations_Internal", (void*)CoreInternalCalls::AsepriteData_GetAnimations);
    mono_add_internal_call("Steel.AsepriteData::CreateEntityFromAsepriteData_Internal", (void*)CoreInternalCalls::AsepriteData_CreateEntityFromAsepriteData);
    mono_add_internal_call("Steel.Material::GetShader_Internal", (void*)CoreInternalCalls::Material_GetShader);
    mono_add_internal_call("Steel.Material::SetShader_Internal", (void*)CoreInternalCalls::Material_SetShader);
    mono_add_internal_call("Steel.Material::GetProperties_Internal", (void*)CoreInternalCalls::Material_GetProperties);
    mono_add_internal_call("Steel.Material::SetProperties_Internal", (void*)CoreInternalCalls::Material_SetProperties);

    mono_add_internal_call("Steel.SceneManager::CreateNewScene_Internal", (void*)CoreInternalCalls::SceneManager_CreateNewScene);
    mono_add_internal_call("Steel.SceneManager::SetActiveScene_Internal", (void*)CoreInternalCalls::SceneManager_SetActiveScene);
    mono_add_internal_call("Steel.SceneManager::SetActiveScene_Internal2", (void*)CoreInternalCalls::SceneManager_SetActiveScene2);
    mono_add_internal_call("Steel.SceneManager::GetActiveScene_Internal", (void*)CoreInternalCalls::SceneManager_GetActiveScene);

    mono_add_internal_call("Steel.Scene::GetName_Internal", (void*)CoreInternalCalls::Scene_GetName);

    mono_add_internal_call("Steel.Entity::CreateNewEntity_Internal", (void*)EntityInternalCalls::Entity_CreateNewEntity);
    mono_add_internal_call("Steel.Entity::CreateNewEntity_Internal2", (void*)EntityInternalCalls::Entity_CreateNewEntity2);
    mono_add_internal_call("Steel.Entity::CreateNewEntity_Internal3", (void*)EntityInternalCalls::Entity_CreateNewEntity3);
    mono_add_internal_call("Steel.Entity::DestroyEntity_Internal", (void*)EntityInternalCalls::Entity_DestroyEntity);
    mono_add_internal_call("Steel.Entity::AddComponent_Internal", (void*)EntityInternalCalls::Entity_AddComponent);
    mono_add_internal_call("Steel.Entity::HasComponent_Internal", (void *) EntityInternalCalls::Entity_HasComponent);
    mono_add_internal_call("Steel.Entity::GetComponent_Internal", (void *) EntityInternalCalls::Entity_GetComponent);
    mono_add_internal_call("Steel.Entity::RemoveComponent_Internal", (void*)EntityInternalCalls::Entity_RemoveComponent);
    mono_add_internal_call("Steel.Entity::GetUUID_Internal", (void*)EntityInternalCalls::Entity_GetUUID);
    mono_add_internal_call("Steel.Entity::GetName_Internal", (void*)EntityInternalCalls::Entity_GetName);
    mono_add_internal_call("Steel.Entity::SetName_Internal", (void*)EntityInternalCalls::Entity_SetName);
    mono_add_internal_call("Steel.Entity::GetParent_Internal", (void*)ComponentsInternalCalls::HierarchyNode_GetParent);
    mono_add_internal_call("Steel.Entity::SetParent_Internal", (void*)ComponentsInternalCalls::HierarchyNode_SetParent);
    mono_add_internal_call("Steel.Entity::GetChildren_Internal", (void*)ComponentsInternalCalls::HierarchyNode_GetChildren);
    mono_add_internal_call("Steel.Entity::GetIsActive_Internal", (void*)EntityInternalCalls::Entity_GetIsActive);
    mono_add_internal_call("Steel.Entity::GetIsActiveSelf_Internal", (void*)EntityInternalCalls::Entity_GetIsActiveSelf);
    mono_add_internal_call("Steel.Entity::SetIsActiveSelf_Internal", (void*)EntityInternalCalls::Entity_SetIsActiveSelf);
    mono_add_internal_call("Steel.Entity::IsDestroyed_Internal", (void*)EntityInternalCalls::Entity_IsDestroyed);

    mono_add_internal_call("Steel.Component::FindAllOfType_Internal", (void*)EntityInternalCalls::Component_FindAllOfType);

    mono_add_internal_call("Steel.Transformation::GetPosition_Internal", (void*)ComponentsInternalCalls::Transformation_GetPosition);
    mono_add_internal_call("Steel.Transformation::SetPosition_Internal", (void*)ComponentsInternalCalls::Transformation_SetPosition);
    mono_add_internal_call("Steel.Transformation::GetLocalPosition_Internal", (void*)ComponentsInternalCalls::Transformation_GetLocalPosition);
    mono_add_internal_call("Steel.Transformation::SetLocalPosition_Internal", (void*)ComponentsInternalCalls::Transformation_SetLocalPosition);
    mono_add_internal_call("Steel.Transformation::GetRotation_Internal", (void*)ComponentsInternalCalls::Transformation_GetRotation);
    mono_add_internal_call("Steel.Transformation::SetRotation_Internal", (void*)ComponentsInternalCalls::Transformation_SetRotation);
    mono_add_internal_call("Steel.Transformation::GetLocalRotation_Internal", (void*)ComponentsInternalCalls::Transformation_GetLocalRotation);
    mono_add_internal_call("Steel.Transformation::SetLocalRotation_Internal", (void*)ComponentsInternalCalls::Transformation_SetLocalRotation);
    mono_add_internal_call("Steel.Transformation::GetScale_Internal", (void*)ComponentsInternalCalls::Transformation_GetScale);
    mono_add_internal_call("Steel.Transformation::SetScale_Internal", (void*)ComponentsInternalCalls::Transformation_SetScale);
    mono_add_internal_call("Steel.Transformation::GetLocalScale_Internal", (void*)ComponentsInternalCalls::Transformation_GetLocalScale);
    mono_add_internal_call("Steel.Transformation::SetLocalScale_Internal", (void*)ComponentsInternalCalls::Transformation_SetLocalScale);

    mono_add_internal_call("Steel.AudioListener::GetVolume_Internal", (void*)ComponentsInternalCalls::AudioListener_GetVolume);
    mono_add_internal_call("Steel.AudioListener::SetVolume_Internal", (void*)ComponentsInternalCalls::AudioListener_SetVolume);
    mono_add_internal_call("Steel.AudioSource::GetVolume_Internal", (void*)ComponentsInternalCalls::AudioSource_GetVolume);
    mono_add_internal_call("Steel.AudioSource::SetVolume_Internal", (void*)ComponentsInternalCalls::AudioSource_SetVolume);
    mono_add_internal_call("Steel.AudioSource::GetLoop_Internal", (void*)ComponentsInternalCalls::AudioSource_GetLoop);
    mono_add_internal_call("Steel.AudioSource::SetLoop_Internal", (void*)ComponentsInternalCalls::AudioSource_SetLoop);
    mono_add_internal_call("Steel.AudioSource::Play_Internal", (void*)ComponentsInternalCalls::AudioSource_Play);
    mono_add_internal_call("Steel.AudioSource::Stop_Internal", (void*)ComponentsInternalCalls::AudioSource_Stop);

    mono_add_internal_call("Steel.SpriteRenderer::GetSprite_Internal", (void*)ComponentsInternalCalls::SpriteRenderer_GetSprite);
    mono_add_internal_call("Steel.SpriteRenderer::SetSprite_Internal", (void*)ComponentsInternalCalls::SpriteRenderer_SetSprite);
    mono_add_internal_call("Steel.SpriteRenderer::GetMaterial_Internal", (void*)ComponentsInternalCalls::SpriteRenderer_GetMaterial);
    mono_add_internal_call("Steel.SpriteRenderer::SetMaterial_Internal", (void*)ComponentsInternalCalls::SpriteRenderer_SetMaterial);
    mono_add_internal_call("Steel.SpriteRenderer::GetCustomMaterialProperties_Internal", (void*)ComponentsInternalCalls::SpriteRenderer_GetCustomMaterialProperties);
    mono_add_internal_call("Steel.SpriteRenderer::SetCustomMaterialProperties_Internal", (void*)ComponentsInternalCalls::SpriteRenderer_SetCustomMaterialProperties);

    mono_add_internal_call("Steel.MeshRenderer::GetMesh_Internal", (void*)ComponentsInternalCalls::MeshRenderer_GetMesh);
    mono_add_internal_call("Steel.MeshRenderer::SetMesh_Internal", (void*)ComponentsInternalCalls::MeshRenderer_SetMesh);
    mono_add_internal_call("Steel.MeshRenderer::GetMaterial_Internal", (void*)ComponentsInternalCalls::MeshRenderer_GetMaterial);
    mono_add_internal_call("Steel.MeshRenderer::SetMaterial_Internal", (void*)ComponentsInternalCalls::MeshRenderer_SetMaterial);
    mono_add_internal_call("Steel.MeshRenderer::GetCustomMaterialProperties_Internal", (void*)ComponentsInternalCalls::MeshRenderer_GetCustomMaterialProperties);
    mono_add_internal_call("Steel.MeshRenderer::SetCustomMaterialProperties_Internal", (void*)ComponentsInternalCalls::MeshRenderer_SetCustomMaterialProperties);

    mono_add_internal_call("Steel.Animator::PlayAnimation_Internal", (void*)ComponentsInternalCalls::Animator_PlayAnimation);
    mono_add_internal_call("Steel.Animator::PlayAnimation_Internal2", (void*)ComponentsInternalCalls::Animator_PlayAnimation2);
    mono_add_internal_call("Steel.Animator::Play_Internal", (void*)ComponentsInternalCalls::Animator_Play);
    mono_add_internal_call("Steel.Animator::Pause_Internal", (void*)ComponentsInternalCalls::Animator_Pause);
    mono_add_internal_call("Steel.Animator::Stop_Internal", (void*)ComponentsInternalCalls::Animator_Stop);
    mono_add_internal_call("Steel.Animator::Restart_Internal", (void*)ComponentsInternalCalls::Animator_Restart);
    mono_add_internal_call("Steel.Animator::AddAnimations_Internal", (void*)ComponentsInternalCalls::Animator_AddAnimations);

    mono_add_internal_call("Steel.Camera::GetEntityWithMainCamera_Internal", (void *) ComponentsInternalCalls::Camera_GetEntityWithMainCamera);
    mono_add_internal_call("Steel.Camera::GetWidth_Internal", (void*)ComponentsInternalCalls::Camera_GetWidth);
    mono_add_internal_call("Steel.Camera::SetWidth_Internal", (void*)ComponentsInternalCalls::Camera_SetWidth);
    mono_add_internal_call("Steel.Camera::GetHeight_Internal", (void*)ComponentsInternalCalls::Camera_GetHeight);
    mono_add_internal_call("Steel.Camera::SetHeight_Internal", (void*)ComponentsInternalCalls::Camera_SetHeight);
    mono_add_internal_call("Steel.Camera::GetNearClippingPlane_Internal", (void*)ComponentsInternalCalls::Camera_GetNearClippingPlane);
    mono_add_internal_call("Steel.Camera::SetNearClippingPlane_Internal", (void*)ComponentsInternalCalls::Camera_SetNearClippingPlane);
    mono_add_internal_call("Steel.Camera::GetFarClippingPlane_Internal", (void*)ComponentsInternalCalls::Camera_GetFarClippingPlane);
    mono_add_internal_call("Steel.Camera::SetFarClippingPlane_Internal", (void*)ComponentsInternalCalls::Camera_SetFarClippingPlane);
    mono_add_internal_call("Steel.Camera::GetResizingMode_Internal", (void*)ComponentsInternalCalls::Camera_GetResizingMode);
    mono_add_internal_call("Steel.Camera::SetResizingMode_Internal", (void*)ComponentsInternalCalls::Camera_SetResizingMode);
    mono_add_internal_call("Steel.Camera::WorldToScreenPoint_Internal", (void*)ComponentsInternalCalls::Camera_WorldToScreenPoint);
    mono_add_internal_call("Steel.Camera::ScreenToWorldPoint_Internal", (void*)ComponentsInternalCalls::Camera_ScreenToWorldPoint);

    mono_add_internal_call("Steel.RigidBody::GetRigidBodyType_Internal", (void*)ComponentsInternalCalls::RigidBody_GetRigidBodyType);
    mono_add_internal_call("Steel.RigidBody::SetRigidBodyType_Internal", (void*)ComponentsInternalCalls::RigidBody_SetRigidBodyType);
    mono_add_internal_call("Steel.RigidBody::GetMass_Internal", (void*)ComponentsInternalCalls::RigidBody_GetMass);
    mono_add_internal_call("Steel.RigidBody::SetMass_Internal", (void*)ComponentsInternalCalls::RigidBody_SetMass);
    mono_add_internal_call("Steel.RigidBody::GetVelocity_Internal", (void*)ComponentsInternalCalls::RigidBody_GetVelocity);
    mono_add_internal_call("Steel.RigidBody::SetVelocity_Internal", (void*)ComponentsInternalCalls::RigidBody_SetVelocity);
    mono_add_internal_call("Steel.RigidBody::GetAngularVelocity_Internal", (void*)ComponentsInternalCalls::RigidBody_GetAngularVelocity);
    mono_add_internal_call("Steel.RigidBody::SetAngularVelocity_Internal", (void*)ComponentsInternalCalls::RigidBody_SetAngularVelocity);
    mono_add_internal_call("Steel.RigidBody::GetGravityScale_Internal", (void*)ComponentsInternalCalls::RigidBody_GetGravityScale);
    mono_add_internal_call("Steel.RigidBody::SetGravityScale_Internal", (void*)ComponentsInternalCalls::RigidBody_SetGravityScale);
    mono_add_internal_call("Steel.RigidBody::GetFriction_Internal", (void*)ComponentsInternalCalls::RigidBody_GetFriction);
    mono_add_internal_call("Steel.RigidBody::SetFriction_Internal", (void*)ComponentsInternalCalls::RigidBody_SetFriction);
    mono_add_internal_call("Steel.RigidBody::GetRestitution_Internal", (void*)ComponentsInternalCalls::RigidBody_GetRestitution);
    mono_add_internal_call("Steel.RigidBody::SetRestitution_Internal", (void*)ComponentsInternalCalls::RigidBody_SetRestitution);
    mono_add_internal_call("Steel.RigidBody::GetLinearDamping_Internal", (void*)ComponentsInternalCalls::RigidBody_GetLinearDamping);
    mono_add_internal_call("Steel.RigidBody::SetLinearDamping_Internal", (void*)ComponentsInternalCalls::RigidBody_SetLinearDamping);
    mono_add_internal_call("Steel.RigidBody::GetAngularDamping_Internal", (void*)ComponentsInternalCalls::RigidBody_GetAngularDamping);
    mono_add_internal_call("Steel.RigidBody::SetAngularDamping_Internal", (void*)ComponentsInternalCalls::RigidBody_SetAngularDamping);
    mono_add_internal_call("Steel.RigidBody::GetFixedRotation_Internal", (void*)ComponentsInternalCalls::RigidBody_GetFixedRotation);
    mono_add_internal_call("Steel.RigidBody::SetFixedRotation_Internal", (void*)ComponentsInternalCalls::RigidBody_SetFixedRotation);
    mono_add_internal_call("Steel.RigidBody::GetUseContinuousCollisionDetection_Internal", (void*)ComponentsInternalCalls::RigidBody_GetUseContinuousCollisionDetection);
    mono_add_internal_call("Steel.RigidBody::SetUseContinuousCollisionDetection_Internal", (void*)ComponentsInternalCalls::RigidBody_SetUseContinuousCollisionDetection);
    mono_add_internal_call("Steel.RigidBody::ApplyForce_Internal", (void*)ComponentsInternalCalls::RigidBody_ApplyForce);
    mono_add_internal_call("Steel.RigidBody::ApplyForce_Internal2", (void*)ComponentsInternalCalls::RigidBody_ApplyForce2);
    mono_add_internal_call("Steel.RigidBody::ApplyTorque_Internal", (void*)ComponentsInternalCalls::RigidBody_ApplyTorque);
    mono_add_internal_call("Steel.RigidBody::ApplyLinearImpulse_Internal", (void*)ComponentsInternalCalls::RigidBody_ApplyLinearImpulse);
    mono_add_internal_call("Steel.RigidBody::ApplyLinearImpulse_Internal2", (void*)ComponentsInternalCalls::RigidBody_ApplyLinearImpulse2);
    mono_add_internal_call("Steel.RigidBody::ApplyAngularImpulse_Internal", (void*)ComponentsInternalCalls::RigidBody_ApplyAngularImpulse);

    mono_add_internal_call("Steel.BoxCollider::GetSize_Internal", (void*)ComponentsInternalCalls::BoxCollider_GetSize);
    mono_add_internal_call("Steel.BoxCollider::SetSize_Internal", (void*)ComponentsInternalCalls::BoxCollider_SetSize);
    mono_add_internal_call("Steel.CircleCollider::GetRadius_Internal", (void*)ComponentsInternalCalls::CircleCollider_GetRadius);
    mono_add_internal_call("Steel.CircleCollider::SetRadius_Internal", (void*)ComponentsInternalCalls::CircleCollider_SetRadius);

    mono_add_internal_call("Steel.RectTransformation::GetAnchorMin_Internal", (void*)UIInternalCalls::RectTransformation_GetAnchorMin);
    mono_add_internal_call("Steel.RectTransformation::SetAnchorMin_Internal", (void*)UIInternalCalls::RectTransformation_SetAnchorMin);
    mono_add_internal_call("Steel.RectTransformation::GetAnchorMax_Internal", (void*)UIInternalCalls::RectTransformation_GetAnchorMax);
    mono_add_internal_call("Steel.RectTransformation::SetAnchorMax_Internal", (void*)UIInternalCalls::RectTransformation_SetAnchorMax);
    mono_add_internal_call("Steel.RectTransformation::GetAnchoredPosition_Internal", (void*)UIInternalCalls::RectTransformation_GetAnchoredPosition);
    mono_add_internal_call("Steel.RectTransformation::SetAnchoredPosition_Internal", (void*)UIInternalCalls::RectTransformation_SetAnchoredPosition);
    mono_add_internal_call("Steel.RectTransformation::GetOffsetMin_Internal", (void*)UIInternalCalls::RectTransformation_GetOffsetMin);
    mono_add_internal_call("Steel.RectTransformation::SetOffsetMin_Internal", (void*)UIInternalCalls::RectTransformation_SetOffsetMin);
    mono_add_internal_call("Steel.RectTransformation::GetOffsetMax_Internal", (void*)UIInternalCalls::RectTransformation_GetOffsetMax);
    mono_add_internal_call("Steel.RectTransformation::SetOffsetMax_Internal", (void*)UIInternalCalls::RectTransformation_SetOffsetMax);
    mono_add_internal_call("Steel.RectTransformation::GetPivot_Internal", (void*)UIInternalCalls::RectTransformation_GetPivot);
    mono_add_internal_call("Steel.RectTransformation::SetPivot_Internal", (void*)UIInternalCalls::RectTransformation_SetPivot);
    mono_add_internal_call("Steel.RectTransformation::GetSize_Internal", (void*)UIInternalCalls::RectTransformation_GetSize);
    mono_add_internal_call("Steel.RectTransformation::SetSize_Internal", (void*)UIInternalCalls::RectTransformation_SetSize);
    mono_add_internal_call("Steel.RectTransformation::GetSortingOrder_Internal", (void*)UIInternalCalls::RectTransformation_GetSortingOrder);
    mono_add_internal_call("Steel.RectTransformation::GetRotation_Internal", (void*)UIInternalCalls::RectTransformation_GetRotation);
    mono_add_internal_call("Steel.RectTransformation::SetRotation_Internal", (void*)UIInternalCalls::RectTransformation_SetRotation);

    mono_add_internal_call("Steel.UIImage::GetSprite_Internal", (void*)UIInternalCalls::UIImage_GetSprite);
    mono_add_internal_call("Steel.UIImage::SetSprite_Internal", (void*)UIInternalCalls::UIImage_SetSprite);
    mono_add_internal_call("Steel.UIImage::GetMaterial_Internal", (void*)UIInternalCalls::UIImage_GetMaterial);
    mono_add_internal_call("Steel.UIImage::SetMaterial_Internal", (void*)UIInternalCalls::UIImage_SetMaterial);
    mono_add_internal_call("Steel.UIImage::GetColor_Internal", (void*)UIInternalCalls::UIImage_GetColor);
    mono_add_internal_call("Steel.UIImage::SetColor_Internal", (void*)UIInternalCalls::UIImage_SetColor);
    mono_add_internal_call("Steel.UIImage::GetConsumeEvents_Internal", (void*)UIInternalCalls::UIImage_GetConsumeEvents);
    mono_add_internal_call("Steel.UIImage::SetConsumeEvents_Internal", (void*)UIInternalCalls::UIImage_SetConsumeEvents);
    mono_add_internal_call("Steel.UIImage::GetCustomMaterialProperties_Internal", (void*)UIInternalCalls::UIImage_GetCustomMaterialProperties);
    mono_add_internal_call("Steel.UIImage::SetCustomMaterialProperties_Internal", (void*)UIInternalCalls::UIImage_SetCustomMaterialProperties);

    mono_add_internal_call("Steel.UIButton::GetTargetImage_Internal", (void*)UIInternalCalls::UIButton_GetTargetImage);
    mono_add_internal_call("Steel.UIButton::SetTargetImage_Internal", (void*)UIInternalCalls::UIButton_SetTargetImage);
    mono_add_internal_call("Steel.UIButton::GetInteractable_Internal", (void*)UIInternalCalls::UIButton_GetInteractable);
    mono_add_internal_call("Steel.UIButton::SetInteractable_Internal", (void*)UIInternalCalls::UIButton_SetInteractable);
    mono_add_internal_call("Steel.UIButton::GetTransition_Internal", (void*)UIInternalCalls::UIButton_GetTransition);
    mono_add_internal_call("Steel.UIButton::SetTransition_Internal", (void*)UIInternalCalls::UIButton_SetTransition);

    mono_add_internal_call("Steel.UIText::GetText_Internal", (void*)UIInternalCalls::UIText_GetText);
    mono_add_internal_call("Steel.UIText::SetText_Internal", (void*)UIInternalCalls::UIText_SetText);
    mono_add_internal_call("Steel.UIText::GetTextSize_Internal", (void*)UIInternalCalls::UIText_GetTextSize);
    mono_add_internal_call("Steel.UIText::SetTextSize_Internal", (void*)UIInternalCalls::UIText_SetTextSize);
    mono_add_internal_call("Steel.UIText::GetLineSpacing_Internal", (void*)UIInternalCalls::UIText_GetLineSpacing);
    mono_add_internal_call("Steel.UIText::SetLineSpacing_Internal", (void*)UIInternalCalls::UIText_SetLineSpacing);
    mono_add_internal_call("Steel.UIText::GetColor_Internal", (void*)UIInternalCalls::UIText_GetColor);
    mono_add_internal_call("Steel.UIText::SetColor_Internal", (void*)UIInternalCalls::UIText_SetColor);
    mono_add_internal_call("Steel.UIText::GetIsTextAutoSize_Internal", (void*)UIInternalCalls::UIButton_GetIsTextAutoSize);
    mono_add_internal_call("Steel.UIText::SetIsTextAutoSize_Internal", (void*)UIInternalCalls::UIButton_SetIsTextAutoSize);
    mono_add_internal_call("Steel.UIText::GetTextAlignment_Internal", (void*)UIInternalCalls::UIButton_GetTextAlignment);
    mono_add_internal_call("Steel.UIText::SetTextAlignment_Internal", (void*)UIInternalCalls::UIButton_SetTextAlignment);
    mono_add_internal_call("Steel.UIText::GetTextOverflowMode_Internal", (void*)UIInternalCalls::UIButton_GetTextOverflowMode);
    mono_add_internal_call("Steel.UIText::SetTextOverflowMode_Internal", (void*)UIInternalCalls::UIButton_SetTextOverflowMode);

    mono_add_internal_call("Steel.UIInputField::GetTargetText_Internal", (void*)UIInternalCalls::UIInputField_GetTargetText);
    mono_add_internal_call("Steel.UIInputField::SetTargetText_Internal", (void*)UIInternalCalls::UIInputField_SetTargetText);
    mono_add_internal_call("Steel.UIInputField::GetTargetImage_Internal", (void*)UIInternalCalls::UIInputField_GetTargetImage);
    mono_add_internal_call("Steel.UIInputField::SetTargetImage_Internal", (void*)UIInternalCalls::UIInputField_SetTargetImage);
    mono_add_internal_call("Steel.UIInputField::GetInteractable_Internal", (void*)UIInternalCalls::UIInputField_GetInteractable);
    mono_add_internal_call("Steel.UIInputField::SetInteractable_Internal", (void*)UIInternalCalls::UIInputField_SetInteractable);
    mono_add_internal_call("Steel.UIInputField::GetCursorWidth_Internal", (void*)UIInternalCalls::UIInputField_GetCursorWidth);
    mono_add_internal_call("Steel.UIInputField::SetCursorWidth_Internal", (void*)UIInternalCalls::UIInputField_SetCursorWidth);
    mono_add_internal_call("Steel.UIInputField::GetCursorColor_Internal", (void*)UIInternalCalls::UIInputField_GetCursorColor);
    mono_add_internal_call("Steel.UIInputField::SetCursorColor_Internal", (void*)UIInternalCalls::UIInputField_SetCursorColor);
    mono_add_internal_call("Steel.UIInputField::GetCursorAutoColor_Internal", (void*)UIInternalCalls::UIInputField_GetAutoColor);
    mono_add_internal_call("Steel.UIInputField::SetCursorAutoColor_Internal", (void*)UIInternalCalls::UIInputField_SetAutoColor);
    mono_add_internal_call("Steel.UIInputField::GetIsMultiline_Internal", (void*)UIInternalCalls::UIInputField_GetIsMultiline);
    mono_add_internal_call("Steel.UIInputField::SetIsMultiline_Internal", (void*)UIInternalCalls::UIInputField_SetIsMultiline);
    mono_add_internal_call("Steel.UIInputField::GetTextType_Internal", (void*)UIInternalCalls::UIInputField_GetTextType);
    mono_add_internal_call("Steel.UIInputField::SetTextType_Internal", (void*)UIInternalCalls::UIInputField_SetTextType);
    mono_add_internal_call("Steel.UIInputField::GetSelectionColor_Internal", (void*)UIInternalCalls::UIInputField_GetSelectionColor);
    mono_add_internal_call("Steel.UIInputField::SetSelectionColor_Internal", (void*)UIInternalCalls::UIInputField_SetSelectionColor);

    mono_add_internal_call("Steel.UICheckBox::GetValue_Internal", (void*)UIInternalCalls::UICheckBox_GetValue);
    mono_add_internal_call("Steel.UICheckBox::SetValue_Internal", (void*)UIInternalCalls::UICheckBox_SetValue);
    mono_add_internal_call("Steel.UICheckBox::GetInteractable_Internal", (void*)UIInternalCalls::UICheckBox_GetInteractable);
    mono_add_internal_call("Steel.UICheckBox::SetInteractable_Internal", (void*)UIInternalCalls::UICheckBox_SetInteractable);
    mono_add_internal_call("Steel.UICheckBox::GetTransition_Internal", (void*)UIInternalCalls::UICheckBox_GetTransition);
    mono_add_internal_call("Steel.UICheckBox::SetTransition_Internal", (void*)UIInternalCalls::UICheckBox_SetTransition);

    mono_add_internal_call("Steel.UITabs::TabsCount_Internal", (void*)UIInternalCalls::UITabs_TabsCount);
    mono_add_internal_call("Steel.UITabs::GetActiveTab_Internal", (void*)UIInternalCalls::UITabs_GetActiveTab);
    mono_add_internal_call("Steel.UITabs::SetActiveTab_Internal", (void*)UIInternalCalls::UITabs_SetActiveTab);
    mono_add_internal_call("Steel.UITabs::GetTab_Internal", (void*)UIInternalCalls::UITabs_GetTab);
    mono_add_internal_call("Steel.UITabs::AddTab_Internal", (void*)UIInternalCalls::UITabs_AddTab);
    mono_add_internal_call("Steel.UITabs::RemoveTab_Internal", (void*)UIInternalCalls::UITabs_RemoveTab);

    mono_add_internal_call("Steel.UILayoutGroup::AddElement_Internal", (void*)UIInternalCalls::UILayoutGroup_AddElement);
    mono_add_internal_call("Steel.UILayoutGroup::AddElement_Internal2", (void*)UIInternalCalls::UILayoutGroup_AddElement2);

    mono_add_internal_call("Steel.UI::CreateUIElement_Internal", (void*)UIInternalCalls::UI_CreateUIElement);
    mono_add_internal_call("Steel.UI::CreateUIElement_Internal2", (void*)UIInternalCalls::UI_CreateUIElement2);
    mono_add_internal_call("Steel.UI::CreateUIElement_Internal3", (void*)UIInternalCalls::UI_CreateUIElement3);
    mono_add_internal_call("Steel.UI::CreateUIImage_Internal", (void*)UIInternalCalls::UI_CreateUIImage);
    mono_add_internal_call("Steel.UI::CreateUIImage_Internal2", (void*)UIInternalCalls::UI_CreateUIImage2);
    mono_add_internal_call("Steel.UI::CreateUIButton_Internal", (void*)UIInternalCalls::UI_CreateUIButton);
    mono_add_internal_call("Steel.UI::CreateUIButton_Internal2", (void*)UIInternalCalls::UI_CreateUIButton2);
    mono_add_internal_call("Steel.UI::CreateUIText_Internal", (void*)UIInternalCalls::UI_CreateUIText);
    mono_add_internal_call("Steel.UI::CreateUIText_Internal2", (void*)UIInternalCalls::UI_CreateUIText2);
    mono_add_internal_call("Steel.UI::CreateUIInputField_Internal", (void*)UIInternalCalls::UI_CreateUIInputField);
    mono_add_internal_call("Steel.UI::CreateUIInputField_Internal2", (void*)UIInternalCalls::UI_CreateUIInputField2);
    mono_add_internal_call("Steel.UI::CreateUIClipping_Internal", (void*)UIInternalCalls::UI_CreateUIClipping);
    mono_add_internal_call("Steel.UI::CreateUIClipping_Internal2", (void*)UIInternalCalls::UI_CreateUIClipping2);
    mono_add_internal_call("Steel.UI::CreateUICheckBox_Internal", (void*)UIInternalCalls::UI_CreateUICheckBox);
    mono_add_internal_call("Steel.UI::CreateUICheckBox_Internal2", (void*)UIInternalCalls::UI_CreateUICheckBox2);
    mono_add_internal_call("Steel.UI::CreateUITabs_Internal", (void*)UIInternalCalls::UI_CreateUITabs);
    mono_add_internal_call("Steel.UI::CreateUITabs_Internal2", (void*)UIInternalCalls::UI_CreateUITabs2);
    mono_add_internal_call("Steel.UI::CreateUILayoutGroup_Internal", (void*)UIInternalCalls::UI_CreateUILayoutGroup);
    mono_add_internal_call("Steel.UI::CreateUILayoutGroup_Internal2", (void*)UIInternalCalls::UI_CreateUILayoutGroup2);
    mono_add_internal_call("Steel.UI::IsPointerOverUI_Internal", (void*)UIInternalCalls::UI_IsPointerOverUI);
}
