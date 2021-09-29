#include <box2d/box2d.h>

#include "../Rendering/SpriteRenderer.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/Transformation.h"
#include "BoxCollider.h"
#include "PhysicsInfo.h"

void BoxCollider::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    info = new BoxCollider::BoxColliderInfo();
    info->BoxShape = new b2PolygonShape();
    SetSizeAutomatically();
}

void BoxCollider::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (info != nullptr)
        delete info->BoxShape;
    delete info;
}

void BoxCollider::SetSizeAutomatically()
{
    glm::vec2 size;
    if (HasComponentS<SpriteRenderer>(Owner))
    {
        auto& sr = GetComponentS<SpriteRenderer>(Owner);
        // We can in theory use sprite's pivot as offset, but that seems unnecessary for the most cases
        size = sr.GetWorldSize();
    }
    else
        size = GetComponentS<Transformation>(Owner).GetScale();

    if (std::abs(size.x) > SHAPE_EPS && std::abs(size.y) > SHAPE_EPS)
    {
        _size = size;
        info->BoxShape->SetAsBox(_size.x * 0.5f, _size.y * 0.5f);
    }
}

glm::vec2 BoxCollider::GetSize() const
{
    return _size;
}

void BoxCollider::SetSize(glm::vec2 size)
{
    if (std::abs(size.x) > SHAPE_EPS && std::abs(size.y) > SHAPE_EPS)
    {
        _size = size;
        info->BoxShape->SetAsBox(_size.x * 0.5f, _size.y * 0.5f);
    }
}
