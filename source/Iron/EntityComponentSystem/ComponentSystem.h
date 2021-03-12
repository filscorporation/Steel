#pragma once

#include "Entity.h"

template <typename T>
class ComponentSystem
{
public:
    virtual void OnComponentAdded(EntityID entityID, T& component) = 0;
    virtual void OnComponentRemoved(EntityID entityID, T& component) = 0;
};
