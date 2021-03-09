#include "Component.h"
#include "../Math/Random.h"

Component::Component(EntityID ownerEntityID)
{
    Owner = ownerEntityID;
}
