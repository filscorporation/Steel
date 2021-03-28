#pragma once

#include <cstdint>

#include "Entity.h"

class Component
{
public:
    explicit Component(EntityID ownerEntityID);
    virtual ~Component() = default;

    bool IsAlive() const;

    EntityID Owner;
};
