#pragma once

#include "Steel/EntityComponentSystem/Component.h"

class TestComponent2 : public Component
{
public:
    explicit TestComponent2(EntityID ownerEntityID) : Component(ownerEntityID) { }

    int IntegerValue = 0;
};
