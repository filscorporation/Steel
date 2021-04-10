#pragma once

#include "Entity.h"

class EntitiesRegistry;

template <typename T>
class ComponentSystem
{
public:
    virtual void OnComponentAdded(EntityID entityID, T& component) = 0;
    virtual void OnComponentRemoved(EntityID entityID, T& component) = 0;

    virtual void OnEntityEnabled(EntityID entityID, T& component) = 0;
    virtual void OnEntityDisabled(EntityID entityID, T& component) = 0;

protected:
    EntitiesRegistry* Registry;

    friend class EntitiesRegistry;
};
