#pragma once

#include <vector>

#include "../Core/Log.h"
#include "Entity.h"

template <typename T>
class SparseDataSet
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
        dense[sparse[id]] = NULL_ENTITY;
        data[sparse[id]].Owner = NULL_ENTITY;
        holes ++;
    }

    void Condense()
    {
        if (holes == 0)
            return;

        int denseSize = dense.size();
        for (int i = 0; i < denseSize; ++i)
        {
            if (dense[i] == NULL_ENTITY)
            {
                Move(size - 1, i);
                size --;
                data.erase(data.end() - 1);
                dense.erase(dense.end() - 1);
                denseSize--;
                i--;
            }
        }

        holes = 0;
    }

private:
    EntityID size = 0;
    uint32_t holes = 0;
    std::vector<T> data;
    std::vector<EntityID> dense;
    std::vector<EntityID> sparse;

    inline void Move(EntityID from, EntityID to)
    {
        if (from == to)
            return;

        dense[to] = dense[from];
        data[to] = data[from];
        if (dense[from] != NULL_ENTITY)
            sparse[dense[to]] = to;
    }

    inline void Swap(EntityID from, EntityID to)
    {
        if (from == to)
            return;

        std::swap(dense[to], dense[from]);
        std::swap(data[to], data[from]);
        sparse[dense[to]] = to;
        if (dense[from] != NULL_ENTITY)
            sparse[dense[from]] = from;
    }

    friend class EntitiesRegistry;
};
