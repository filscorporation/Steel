#pragma once

#include "../EntityComponentSystem/Component.h"

class Collider : public Component
{
public:
    explicit Collider(EntityID ownerEntityID) : Component(ownerEntityID) { }
};
