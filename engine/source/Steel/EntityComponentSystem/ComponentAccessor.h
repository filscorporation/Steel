#pragma once

#include "ComponentsPoolWrapper.h"

// Allows access to certain component type without using components types hashmap
template <typename T>
class ComponentAccessor
{
public:
    explicit ComponentAccessor(ComponentsPoolWrapper<T>* componentsPool) { _componentsPool = componentsPool; }

    bool Has(EntityID entityID) const
    {
        if (_componentsPool == nullptr)
            return false;
        EntityID id = Entity::EntityIDGetID(entityID);
        return _componentsPool->Storage.Has(id);
    }

    T& Get(EntityID entityID) const
    {
        EntityID id = Entity::EntityIDGetID(entityID);
        return _componentsPool->Storage.Get(id);
    }

    bool HasInactive(EntityID entityID) const
    {
        if (_componentsPool == nullptr)
            return false;
        EntityID id = Entity::EntityIDGetID(entityID);
        return _componentsPool->InactiveStorage.Has(id);
    }

    T& GetInactive(EntityID entityID) const
    {
        EntityID id = Entity::EntityIDGetID(entityID);
        return _componentsPool->InactiveStorage.Get(id);
    }

private:
    ComponentsPoolWrapper<T>* _componentsPool;
};
