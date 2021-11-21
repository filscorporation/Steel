#pragma once

#include "../EntityComponentSystem/Component.h"

class Collider : public Component
{
    // TODO: not used
public:
    explicit Collider(EntityID ownerEntityID) : Component(ownerEntityID) { }
};
