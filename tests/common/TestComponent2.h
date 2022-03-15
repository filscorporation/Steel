#pragma once

#include "Steel/EntityComponentSystem/Component.h"

class TestComponent2 : public Component
{
    DEFINE_COMPONENT(TestComponent2)

public:
    explicit TestComponent2(EntityID ownerEntityID) : Component(ownerEntityID) { }

    int IntegerValue = 0;
};
