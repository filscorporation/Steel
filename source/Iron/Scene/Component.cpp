#include "Component.h"
#include "../Math/Random.h"

Component::Component()
{
    ID = Random::NextUint();
}
