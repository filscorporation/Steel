#include <box2d/box2d.h>
#include "BoxCollider.h"
#include "../Scene/Object.h"
#include "../Rendering/SpriteRenderer.h"

struct BoxCollider::BoxColliderInfo
{
    b2PolygonShape* GroundBox;
};

BoxCollider::BoxCollider()
{
    info = new BoxColliderInfo();

    glm::vec2 size;
    auto sr = ParentObject->GetComponent<SpriteRenderer>();
    if (sr != nullptr)
    {
        size = sr->GetWorldSize();
    }
    info->GroundBox->SetAsBox(size.x * 0.5f, size.y * 0.5f);
}

BoxCollider::~BoxCollider()
{

}
