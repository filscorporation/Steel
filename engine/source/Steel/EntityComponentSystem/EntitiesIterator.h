#pragma once

#include "ComponentsPoolWrapper.h"

// Allows iterating over entities with certain component
class EntitiesIterator
{
public:
    explicit EntitiesIterator(ComponentsPoolWrapperBase* pool, bool active) { _pool = pool; _active = active; }

    EntityID operator[] (int index)
    {
        return _pool->GetOwnerByIndex(index, _active);
    }

    int Size()
    {
        return _pool == nullptr ? 0 : _pool->Size(_active);
    }

protected:
    ComponentsPoolWrapperBase* _pool;
    bool _active;
};
