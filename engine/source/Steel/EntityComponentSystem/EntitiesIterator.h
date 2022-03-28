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

    // EntitiesIterator will return size including elements deleted after last condense was invoked,
    // so it is important to check for NULL_ENTITY on every result to skip entities with removed component
    int Size()
    {
        return _pool == nullptr ? 0 : _pool->Size(_active);
    }

    // Elements count excluding holes (shouldn't be used for iteration)
    int CondensedSize()
    {
        return _pool == nullptr ? 0 : _pool->CondensedSize(_active);
    }

protected:
    ComponentsPoolWrapperBase* _pool;
    bool _active;
};
