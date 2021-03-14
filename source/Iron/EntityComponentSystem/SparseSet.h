#pragma once

#include <vector>

#include "../Core/Log.h"
#include "Entity.h"

template <typename T>
class SparseSet
{
public:
    using Iterator = typename std::vector<T>::iterator;
    Iterator begin() { return data.begin(); }
    Iterator end() { return data.end(); }

    T& operator[] (int index)
    {
        return data[index];
    }

    int Size()
    {
        return size;
    }

    T& Get(EntityID id)
    {
        return data[sparse[id]];
    }

    bool Has(EntityID id)
    {
        EntityID s_id;
        return sparse.size() > id && dense.size() > (s_id = sparse[id]) && dense[s_id] == id;
    }

    T& Add(EntityID id, EntityID param)
    {
        // TODO: passing 2 parameters is bad
        data.emplace_back(param);
        dense.emplace_back(id);
        if (sparse.size() < id + 1)
            sparse.resize(id + 1);
        sparse[id] = size;
        size++;

        return data[size - 1];
    }

    void Remove(EntityID id)
    {
        Move(size - 1, sparse[id]);
        size --;
        data.erase(data.end() - 1);
        dense.erase(dense.end() - 1);
    }

private:
    EntityID size = 0;
    std::vector<T> data;
    std::vector<EntityID> dense;
    std::vector<EntityID> sparse;

    inline void Move(EntityID from, EntityID to)
    {
        if (from == to)
            return;

        dense[to] = dense[from];
        data[to] = data[from];
        sparse[dense[to]] = to;
    }
};
