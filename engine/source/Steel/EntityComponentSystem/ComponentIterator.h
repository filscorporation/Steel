#pragma once

#include "ComponentsPoolWrapper.h"

// Allows iterating over certain component in entities
template <typename T>
class ComponentIterator
{
public:
    explicit ComponentIterator(ComponentsPoolWrapper<T>* pool, bool active) { _pool = pool; _active = active; }

    T& operator[] (int index)
    {
        return _active ? _pool->Storage[index] : _pool->InactiveStorage[index];
    }

    // ComponentIterator will return size including elements deleted after last condense was invoked,
    // so it is important to check IsAlive() on every result to skip removed components
    int Size()
    {
        return _pool == nullptr ? 0 : (_active ? _pool->Storage.Size() : _pool->InactiveStorage.Size());
    }

    using Iterator = typename ComponentsPool<T>::Iterator;
    Iterator begin() { return _active ? _pool->Storage.begin() : _pool->InactiveStorage.begin(); }
    Iterator end() { return _active ? _pool->Storage.end() : _pool->InactiveStorage.end(); }

protected:
    ComponentsPoolWrapper<T>* _pool;
    bool _active;
};
