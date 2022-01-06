#include <box2d/box2d.h>

#include "../Rendering/SpriteRenderer.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/Transformation.h"
#include "CircleCollider.h"
#include "PhysicsCore.h"
#include "PhysicsInfo.h"

void CircleCollider::RegisterType()
{
    REGISTER_TYPE(CircleCollider);
    // TODO
}

void CircleCollider::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (!PhysicsCore::Initialized())
        return;

    info = new CircleCollider::CircleColliderInfo();
    info->CircleShape = new b2CircleShape();
    SetSizeAutomatically();
}

void CircleCollider::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (info != nullptr)
        delete info->CircleShape;
    delete info;
}

void CircleCollider::Init()
{
    info = new CircleCollider::CircleColliderInfo();
    info->CircleShape = new b2CircleShape();
    SetSizeAutomatically();
}

void CircleCollider::SetSizeAutomatically()
{
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

    if (std::abs(size.x) > SHAPE_EPS && std::abs(size.y) > SHAPE_EPS)
    {
        info->CircleShape->m_p = b2Vec2(0.0f, 0.0f);
        if (autoSize)
            _radius = (size.x > size.y ? size.x : size.y) * 0.5f;
        info->CircleShape->m_radius = _radius;
    }
}

float CircleCollider::GetRadius() const
{
    return _radius;
}

void CircleCollider::SetRadius(float radius)
{
    if (std::abs(_radius) > SHAPE_EPS)
    {
        _radius = radius;
        autoSize = false;

        if (PhysicsCore::Initialized())
            info->CircleShape->m_radius = _radius;
    }
}
