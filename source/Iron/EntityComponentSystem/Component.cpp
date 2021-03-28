#include "Component.h"

Component::Component(EntityID ownerEntityID)
{
    Owner = ownerEntityID;
}

bool Component::IsAlive() const
{
    return Owner != NULL_ENTITY;
}
