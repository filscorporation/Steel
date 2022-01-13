#include <box2d/box2d.h>

#include "BoxCollider.h"
#include "PhysicsCore.h"
#include "PhysicsInfo.h"
#include "Steel/Rendering/SpriteRenderer.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Scene/Transformation.h"

void BoxCollider::RegisterType()
{
    REGISTER_TYPE(BoxCollider);
    REGISTER_ATTRIBUTE(BoxCollider, "size", GetSize, SetSize, glm::vec2, AttributeFlags::Public);
}

void BoxCollider::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    // TODO: probably temporary before serialization copy
    if (info != nullptr)
        return;

    if (!PhysicsCore::Initialized())
        return;

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

void BoxCollider::ApplyPhysicsProperties()
{
    info = new BoxCollider::BoxColliderInfo();
    info->BoxShape = new b2PolygonShape();
    SetSizeAutomatically();
}

void BoxCollider::SetSizeAutomatically()
{
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

    if (std::abs(size.x) > SHAPE_EPS && std::abs(size.y) > SHAPE_EPS)
    {
        _size = size;
        info->BoxShape->SetAsBox(_size.x * 0.5f, _size.y * 0.5f);
    }
}

const glm::vec2& BoxCollider::GetSize() const
{
    return _size;
}

void BoxCollider::SetSize(const glm::vec2& size)
{
    if (std::abs(size.x) > SHAPE_EPS && std::abs(size.y) > SHAPE_EPS)
    {
        _size = size;
        autoSize = false;

        if (PhysicsCore::Initialized())
            info->BoxShape->SetAsBox(_size.x * 0.5f, _size.y * 0.5f);
    }
}
