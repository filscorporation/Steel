#include <box2d/box2d.h>

#include "../Rendering/SpriteRenderer.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/Transformation.h"
#include "BoxCollider.h"
#include "PhysicsInfo.h"

void BoxCollider::SetSizeAutomatically()
{
    if (HasComponentS<SpriteRenderer>(Owner))
    {
        auto& sr = GetComponentS<SpriteRenderer>(Owner);
        _size = sr.GetWorldSize();
    }
    else
        _size = GetComponentS<Transformation>(Owner).GetScale();
    info->BoxShape->SetAsBox(_size.x * 0.5f, _size.y * 0.5f);
}

glm::vec2 BoxCollider::GetSize()
{
    return _size;
}

void BoxCollider::SetSize(glm::vec2 size)
{
    _size = size;
    info->BoxShape->SetAsBox(_size.x * 0.5f, _size.y * 0.5f);
}
