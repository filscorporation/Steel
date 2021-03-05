#include <box2d/box2d.h>
#include "BoxCollider.h"
#include "../Scene/Entity.h"
#include "../Rendering/SpriteRenderer.h"
#include "PhysicsInfo.h"

BoxCollider::BoxCollider(Entity* parentEntity) : Collider(parentEntity)
{
    info = new BoxColliderInfo();
    info->GroundBox = new b2PolygonShape();
    SetSizeAutomatically();
}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::SetSizeAutomatically()
{
    auto sr = ParentEntity->GetComponent<SpriteRenderer>();
    if (sr != nullptr)
    {
        _size = sr->GetWorldSize();
    }
    else
        _size = ParentEntity->Transform->GetScale();
    info->GroundBox->SetAsBox(_size.x * 0.5f, _size.y * 0.5f);
}

glm::vec2 BoxCollider::GetSize()
{
    return _size;
}

void BoxCollider::SetSize(glm::vec2 size)
{
    _size = size;
    info->GroundBox->SetAsBox(_size.x * 0.5f, _size.y * 0.5f);
}
