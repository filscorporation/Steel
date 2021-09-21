#pragma once

#include <vector>

#include "Entity.h"

class SparseSet
{
public:
    using Iterator = typename std::vector<EntityID>::iterator;
    Iterator begin() { return dense.begin(); }
    Iterator end() { return dense.end(); }

    EntityID operator[] (int index)
    {
        return dense[index];
    }

    int Size()
    {
        return size;
    }

    EntityID Get(EntityID id)
    {
        return dense[sparse[id]];
    }

    bool Has(EntityID id)
    {
        EntityID s_id;
        return sparse.size() > id && dense.size() > (s_id = sparse[id]) && dense[s_id] == id;
    }

    void Add(EntityID id)
    {
        dense.emplace_back(id);
        if (sparse.size() < id + 1)
            sparse.resize(id + 1);
        sparse[id] = size;
        size++;
    }

    void Remove(EntityID id)
    {
        Move(size - 1, sparse[id]);
        size --;
        dense.erase(dense.end() - 1);
    }

private:
    EntityID size = 0;
    std::vector<EntityID> dense;
    std::vector<EntityID> sparse;

    inline void Move(EntityID from, EntityID to)
    {
        if (from == to)
            return;

        dense[to] = dense[from];
        sparse[dense[to]] = to;
    }
};
