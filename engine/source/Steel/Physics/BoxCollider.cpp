#include "BoxCollider.h"
#include "PhysicsCore.h"
#include "PhysicsInfo.h"
#include "Steel/Rendering/SpriteRenderer.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scene/Transformation.h"
#include "Steel/Serialization/AttributesRegistration.h"

#include <box2d/box2d.h>

void BoxCollider::RegisterType()
{
    REGISTER_COMPONENT(BoxCollider);
    REGISTER_ATTRIBUTE(BoxCollider, "size", GetSize, SetSize, glm::vec2, AttributeFlags::Public, "Size");
}

void BoxCollider::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (PhysicsCore::Initialized() && entitiesRegistry->EntityGetState(Owner) & EntityStates::IsActive)
    {
        PrepareColliderInfo();
        OnSizeChanged();
    }
}

void BoxCollider::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    if (PhysicsCore::Initialized() && info != nullptr)
    {
        ApplyPhysicsProperties();
        OnSizeChanged();
    }
}

void BoxCollider::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (info != nullptr)
        delete info->BoxShape;
    delete info;
}

void BoxCollider::SetSizeAutomatically()
{
    glm::vec2 oldSize = _size;
    glm::vec2 size;
    if (autoSize)
    {
        if (HasComponentS<SpriteRenderer>(Owner))
        {
            auto& sr = GetComponentS<SpriteRenderer>(Owner);
            // We can in theory use sprite's pivot as offset, but that seems unnecessary for the most cases
            size = sr.GetWorldSize();
        }
        else
            size = GetComponentS<Transformation>(Owner).GetScale();
    }
    else
        size = _size;

    _size = glm::vec2(std::abs(size.x), std::abs(size.y));
    info->BoxShape->SetAsBox(_size.x * 0.5f, _size.y * 0.5f);

    if (std::abs(oldSize.x - _size.x) > SHAPE_EPS || std::abs(oldSize.y - _size.y) > SHAPE_EPS)
        OnSizeChanged();
}

const glm::vec2& BoxCollider::GetSize() const
{
    return _size;
}

void BoxCollider::SetSize(const glm::vec2& size)
{
    _size = glm::vec2(std::abs(size.x), std::abs(size.y));
    autoSize = false;

    if (PhysicsCore::Initialized())
    {
        info->BoxShape->SetAsBox(_size.x * 0.5f, _size.y * 0.5f);
        OnSizeChanged();
    }
}

bool BoxCollider::IsSizeValid() const
{
    return std::abs(_size.x) > SHAPE_EPS && std::abs(_size.y) > SHAPE_EPS;
}

void BoxCollider::PrepareColliderInfo()
{
    if (info == nullptr)
    {
        info = new BoxCollider::BoxColliderInfo();
        info->BoxShape = new b2PolygonShape();
    }

    ApplyPhysicsProperties();
}

void BoxCollider::ApplyPhysicsProperties()
{
    SetSizeAutomatically();
}
