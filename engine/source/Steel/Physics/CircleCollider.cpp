#include <box2d/box2d.h>

#include "../Rendering/SpriteRenderer.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/Transformation.h"
#include "CircleCollider.h"
#include "PhysicsInfo.h"

void CircleCollider::OnCreated(EntitiesRegistry* entitiesRegistry)
{
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

void CircleCollider::SetSizeAutomatically()
{
    glm::vec2 size;
    if (HasComponentS<SpriteRenderer>(Owner))
    {
        auto& sr = GetComponentS<SpriteRenderer>(Owner);
        size = sr.GetWorldSize();
    }
    else
        size = GetComponentS<Transformation>(Owner).GetScale();

    if (std::abs(size.x) > SHAPE_EPS && std::abs(size.y) > SHAPE_EPS)
    {
        info->CircleShape->m_p = b2Vec2(0.0f, 0.0f);
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
        info->CircleShape->m_radius = _radius;
    }
}
