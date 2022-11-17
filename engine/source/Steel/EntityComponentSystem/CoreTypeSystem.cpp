#include "CoreTypeSystem.h"
#include "TypeInfoStorage.h"
#include "Steel/Core/Components.h"
#include "Steel/Core/Resources.h"

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
    UIScrollableView::RegisterType();
    UITabs::RegisterType();
    UIText::RegisterType();
    RectTransformation::RegisterType();
    UIEventHandler::RegisterType();

    UITabInfo::RegisterType();
    UILayoutElementInfo::RegisterType();

    Animation::RegisterType();
    AudioTrack::RegisterType();
    Sprite::RegisterType();
    Material::RegisterType();
    Shader::RegisterType();
    Texture::RegisterType();
    AsepriteData::RegisterType();
    SceneData::RegisterType();
    Font::RegisterType();
}

void CoreTypeSystem::Terminate()
{
    TypeInfoStorage::Clear();
}
