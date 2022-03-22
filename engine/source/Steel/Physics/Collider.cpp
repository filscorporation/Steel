#include "Collider.h"
#include "RigidBody.h"
#include "Steel/Scene/SceneHelper.h"

void Collider::OnSizeChanged()
{
    if (HasComponentS<RigidBody>(Owner))
    {
        GetComponentS<RigidBody>(Owner).OnColliderUpdated();
    }
}
