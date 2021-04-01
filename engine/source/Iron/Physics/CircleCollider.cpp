#include <box2d/box2d.h>

#include "../Rendering/SpriteRenderer.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/Transformation.h"
#include "CircleCollider.h"
#include "PhysicsInfo.h"

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
    info->CircleShape->m_p = b2Vec2(0.0f, 0.0f);
    _radius = (size.x > size.y ? size.x : size.y) * 0.5f;
    info->CircleShape->m_radius = _radius;
}

float CircleCollider::GetRadius()
{
    return _radius;
}

void CircleCollider::SetRadius(float radius)
{
    _radius = radius;
    info->CircleShape->m_radius = _radius;
}
