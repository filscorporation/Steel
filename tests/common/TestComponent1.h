#pragma once

#include "Iron.h"

class TestComponent1 : public Component
{
public:
    explicit TestComponent1(EntityID ownerEntityID) : Component(ownerEntityID) { }

    int IntegerValue = 0;
};
