#include "CircleCollider.h"
#include "PhysicsCore.h"
#include "PhysicsInfo.h"
#include "Steel/Rendering/SpriteRenderer.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Serialization/AttributesRegistration.h"

#include <box2d/box2d.h>

void CircleCollider::RegisterType()
{
    REGISTER_COMPONENT(CircleCollider);
    REGISTER_ATTRIBUTE(CircleCollider, "radius", GetRadius, SetRadius, float, AttributeFlags::Public, "Radius");
}

void CircleCollider::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (PhysicsCore::Initialized() && entitiesRegistry->EntityGetState(Owner) & EntityStates::IsActive)
    {
        PrepareColliderInfo();
        OnSizeChanged();
    }
}

void CircleCollider::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    if (PhysicsCore::Initialized() && info != nullptr)
    {
        ApplyPhysicsProperties();
        OnSizeChanged();
    }
}

void CircleCollider::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (info != nullptr)
        delete info->CircleShape;
    delete info;
}

void CircleCollider::SetSizeAutomatically()
{
    float oldRadius = _radius;
    glm::vec2 size;
    if (autoSize)
    {
        if (HasComponentS<SpriteRenderer>(Owner))
        {
            auto& sr = GetComponentS<SpriteRenderer>(Owner);
            size = sr.GetWorldSize();
        }
        else
            size = GetComponentS<Transformation>(Owner).GetScale();
    }
    else
        size = glm::vec2(_radius, _radius);

    size = glm::vec2(std::abs(size.x), std::abs(size.y));
    info->CircleShape->m_p = b2Vec2(0.0f, 0.0f);
    if (autoSize)
        _radius = (size.x > size.y ? size.x : size.y) * 0.5f;
    info->CircleShape->m_radius = _radius;

    if (std::abs(oldRadius - _radius) > SHAPE_EPS)
        OnSizeChanged();
}

float CircleCollider::GetRadius() const
{
    return _radius;
}

void CircleCollider::SetRadius(float radius)
{
    _radius = std::abs(radius);
    autoSize = false;

    if (PhysicsCore::Initialized() && info != nullptr)
    {
        info->CircleShape->m_radius = _radius;
        OnSizeChanged();
    }
}

bool CircleCollider::IsSizeValid() const
{
    return std::abs(_radius) > SHAPE_EPS;
}

void CircleCollider::PrepareColliderInfo()
{
    if (info == nullptr)
    {
        info = new CircleCollider::CircleColliderInfo();
        info->CircleShape = new b2CircleShape();
    }

    ApplyPhysicsProperties();
}

void CircleCollider::ApplyPhysicsProperties()
{
    SetSizeAutomatically();
}
