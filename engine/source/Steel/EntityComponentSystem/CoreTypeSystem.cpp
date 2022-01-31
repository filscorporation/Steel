#include "CoreTypeSystem.h"
#include "Steel/Core/Components.h"

void CoreTypeSystem::Init()
{
    Animator::RegisterType();
    AudioListener::RegisterType();
    AudioSource::RegisterType();
    BoxCollider::RegisterType();
    CircleCollider::RegisterType();
    RigidBody::RegisterType();
    Camera::RegisterType();
    MeshRenderer::RegisterType();
    SpriteRenderer::RegisterType();
    HierarchyNode::RegisterType();
    IDComponent::RegisterType();
    NameComponent::RegisterType();
    Transformation::RegisterType();
    ScriptComponent::RegisterType();
    UIButton::RegisterType();
    UICheckBox::RegisterType();
    UIClipping::RegisterType();
    UIImage::RegisterType();
    UIInputField::RegisterType();
    UILayoutGroup::RegisterType();
    UITabs::RegisterType();
    UIText::RegisterType();
    RectTransformation::RegisterType();
    UIEventHandler::RegisterType();

    UITabInfo::RegisterType();
    UILayoutElementInfo::RegisterType();
}

void CoreTypeSystem::Terminate()
{
    TypeInfoStorage::Clear();
}
