#include "Component.h"
#include "../Math/Random.h"

Component::Component(Entity* parentEntity)
{
    ParentEntity = parentEntity;
    ID = Random::NextUint();
}
