#include <box2d/box2d.h>
#include "BoxCollider.h"
#include "../Scene/Object.h"
#include "../Rendering/SpriteRenderer.h"
#include "PhysicsInfo.h"

BoxCollider::BoxCollider()
{
    info = new BoxColliderInfo();
    info->GroundBox = new b2PolygonShape();
}

BoxCollider::~BoxCollider()
{

}

void BoxCollider::SetSizeAutomatically()
{
    auto sr = ParentObject->GetComponent<SpriteRenderer>();
    if (sr != nullptr)
    {
        _size = sr->GetWorldSize();
    }
    else
        _size = ParentObject->Transform->GetScale();
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
