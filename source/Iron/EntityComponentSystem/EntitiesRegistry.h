#pragma once

#include <typeinfo>
#include <typeindex>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "Entity.h"
#include "SparseSet.h"
#include "ComponentSystem.h"

#define ENTITY_ID_MASK 0xFFFFFu
#define ENTITY_VERSION_MASK 0xFFFu
#define ENTITY_ID_SHIFT 12u

#define TYPE_ID(m_type) std::type_index(typeid(m_type))
using ComponentTypeID = std::type_index;

// Stores all components of one type by entityID
template <typename T>
using ComponentsPool = SparseSet<T>;

// Base class to store components pool
class ComponentsPoolWrapperBase
{
public:
    virtual ~ComponentsPoolWrapperBase() = default;
    virtual void BeforeDeleteByEntityID(EntityID entityID, EntityID id) = 0;
    virtual void DeleteByEntityID(EntityID entityID, EntityID id) = 0;
};

// Wrapper around ComponentsPool
template <typename T>
class ComponentsPoolWrapper : public ComponentsPoolWrapperBase
{
public:
    ComponentsPool<T> Storage;
    ComponentSystem<T>* System = nullptr;

    ~ComponentsPoolWrapper()
    {
        if (System != nullptr)
        {
            for (auto& component : Storage)
            {
                System->OnComponentRemoved(component.Owner, component);
            }
        }
    }

    void BeforeDeleteByEntityID(EntityID entityID, EntityID id) override
    {
        if (!Storage.Has(id))
            return;

        if (System != nullptr)
        {
            auto& component = Storage.Get(id);
            System->OnComponentRemoved(entityID, component);
        }
    }

    void DeleteByEntityID(EntityID entityID, EntityID id) override
    {
        if (!Storage.Has(id))
            return;

        Storage.Remove(id);
    }
};

// Stores all components pools by Component type ID
using ComponentsMap = std::unordered_map<ComponentTypeID, ComponentsPoolWrapperBase*>;

// Allows to iterate over certain component
template <typename T>
struct ComponentIterator
{
public:
    explicit ComponentIterator(ComponentsPoolWrapper<T>* pool) { _pool = pool; }

    T& operator[] (int index)
    {
        return _pool->Storage[index];
    }

    int Size()
    {
        return _pool->Storage.Size();
    }

    using Iterator = typename ComponentsPool<T>::Iterator;
    Iterator begin() { return _pool->Storage.begin(); }
    Iterator end() { return _pool->Storage.end(); }

private:
    ComponentsPoolWrapper<T>* _pool;
};

class EntitiesRegistry
{
public:
    EntitiesRegistry() = default;
    ~EntitiesRegistry()
    {
        isCleared = true;
        CleanAllEntities();
    };

    template<typename T>
    void RegisterSystem(ComponentSystem<T>* system)
    {
        auto typeID = TYPE_ID(T);

        ComponentsPoolWrapper<T>* pool;
        if (componentsMap.find(typeID) == componentsMap.end())
        {
            pool = new ComponentsPoolWrapper<T>();
            componentsMap[typeID] = pool;
        }
        else
        {
            pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];
        }

        if (pool->System != nullptr)
            Log::LogWarning("Replacing component system: " + std::string(typeID.name()));

        pool->System = system;
    }

    template<typename T>
    bool RemoveSystem()
    {
        auto typeID = TYPE_ID(T);

        ComponentsPoolWrapper<T>* pool;
        if (componentsMap.find(typeID) == componentsMap.end())
        {
            return false;
        }
        else
        {
            pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];
        }

        if (pool->System == nullptr)
            return false;

        pool->System = nullptr;
    }

private:
    std::vector<EntityID> entityIDs;
    int freeIDsCount = 0;
    EntityID nextFreeID = 0;

    bool isCleared = false;

    static EntityID EntityIDCombine(EntityID id, EntityID version)
    {
        return (id << ENTITY_ID_SHIFT) + version;
    }

    static EntityID EntityIDIncrementVersion(EntityID id)
    {
        return EntityIDGetVersion(id) == ENTITY_VERSION_MASK
            ? EntityIDCombine(EntityIDGetID(id), 1)
            : id + 1;
    }

    static EntityID EntityIDGetID(EntityID entityID)
    {
        return (entityID & ENTITY_ID_MASK << ENTITY_ID_SHIFT) >> ENTITY_ID_SHIFT;
    }

    static EntityID EntityIDGetVersion(EntityID entityID)
    {
        return entityID & ENTITY_VERSION_MASK;
    }

public:
    EntityID CreateNewEntity()
    {
        EntityID result;
        if (freeIDsCount == 0)
        {
            result = EntityIDCombine(entityIDs.size(), 1);
            entityIDs.emplace_back(result);
        }
        else
        {
            freeIDsCount--;
            result = EntityIDCombine(nextFreeID, EntityIDGetVersion(entityIDs[nextFreeID]));
            if (freeIDsCount > 0)
            {
                nextFreeID = EntityIDGetID(entityIDs[nextFreeID]);
                entityIDs[EntityIDGetID(result)] = result;
            }
        }

        return result;
    }

    bool EntityExists(EntityID entityID)
    {
        return EntityIDGetVersion(entityIDs[EntityIDGetID(entityID)]) == EntityIDGetVersion(entityID);
    }

    void DeleteEntity(EntityID entityID)
    {
        if (!EntityExists(entityID))
            return;

        // Increase this entityID version by one
        entityIDs[EntityIDGetID(entityID)] = EntityIDIncrementVersion(entityID);
        if (freeIDsCount != 0)
        {
            // If there are any free ids already, create link from current deleted id to previously deleted
            entityIDs[EntityIDGetID(entityID)] = EntityIDCombine(nextFreeID, EntityIDGetVersion(entityIDs[EntityIDGetID(entityID)]));
        }
        // Next free id is current deleted
        nextFreeID = EntityIDGetID(entityID);
        freeIDsCount++;

        for (auto component : componentsMap)
        {
            component.second->BeforeDeleteByEntityID(entityID, EntityIDGetID(entityID));
        }
        for (auto component : componentsMap)
        {
            component.second->DeleteByEntityID(entityID, EntityIDGetID(entityID));
        }
    }

    void CleanAllEntities()
    {
        entityIDs.clear();
        nextFreeID = 0;
        freeIDsCount = 0;
        for (auto poolPair : componentsMap)
        {
            delete poolPair.second;
        }
    }

private:
    ComponentsMap componentsMap;

public:
    template <typename T>
    ComponentIterator<T> GetComponentIterator()
    {
        auto typeID = TYPE_ID(T);

        if (componentsMap.find(typeID) == componentsMap.end())
        {
            return ComponentIterator<T>(new ComponentsPoolWrapper<T>());
        }

        return ComponentIterator<T>((ComponentsPoolWrapper<T>*)componentsMap[typeID]);
    }

    template<class T, class Comparer>
    bool SortComponents(Comparer comparer)
    {
        auto typeID = TYPE_ID(T);

        if (componentsMap.find(typeID) == componentsMap.end())
            return false;

        auto pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];
        if (pool->Storage.Size() <= 0)
            return false;

        // Sort
        if (std::is_sorted(pool->Storage.data.begin(), pool->Storage.data.end(), comparer))
            return false;
        std::sort(pool->Storage.begin(), pool->Storage.end(), comparer);

        // Restore links
        for (int i = 0; i < pool->Storage.size; ++i)
        {
            pool->Storage.dense[i] = EntityIDGetID(pool->Storage.data[i].Owner);
            pool->Storage.sparse[pool->Storage.dense[i]] = i;
        }

        return true;
    }

    template<class SourceT, class TargetT>
    void ApplyOrder()
    {
        auto sourceTypeID = TYPE_ID(SourceT);
        auto targetTypeID = TYPE_ID(TargetT);

        if (componentsMap.find(sourceTypeID) == componentsMap.end() || componentsMap.find(targetTypeID) == componentsMap.end())
            return;

        auto sourcePool = (ComponentsPoolWrapper<SourceT>*)componentsMap[sourceTypeID];
        auto targetPool = (ComponentsPoolWrapper<TargetT>*)componentsMap[targetTypeID];
        if (sourcePool->Storage.Size() <= 0 || targetPool->Storage.Size() <= 0)
            return;

        int currentID = 0;
        for (int i = 0; i < sourcePool->Storage.size; ++i)
        {
            EntityID id = EntityIDGetID(sourcePool->Storage.data[i].Owner);
            if (targetPool->Storage.Has(id))
            {
                targetPool->Storage.Swap(targetPool->Storage.sparse[id], currentID);
                currentID++;
            }
        }
    }

    template<class T>
    T& AddComponent(EntityID entityID)
    {
        auto typeID = TYPE_ID(T);
        EntityID id = EntityIDGetID(entityID);

        ComponentsPoolWrapper<T>* pool;
        if (componentsMap.find(typeID) == componentsMap.end())
        {
            pool = new ComponentsPoolWrapper<T>();
            componentsMap[typeID] = pool;
        }
        else
        {
            pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];
        }

        if (pool->Storage.Has(id))
            return pool->Storage.Get(id);

        auto& component = pool->Storage.Add(id, entityID);
        if (pool->System != nullptr)
            pool->System->OnComponentAdded(entityID, component);

        return component;
    }

    template<class T>
    bool HasComponent(EntityID entityID)
    {
        if (isCleared)
            return false;

        auto typeID = TYPE_ID(T);
        EntityID id = EntityIDGetID(entityID);

        ComponentsPoolWrapper<T>* pool;
        if (componentsMap.find(typeID) == componentsMap.end())
            return false;
        else
            pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];

        return pool->Storage.Has(id);
    }

    template<class T>
    T& GetComponent(EntityID entityID)
    {
        EntityID id = EntityIDGetID(entityID);

        // Out of all methods, "get" doesn't check if there is component for the reason of return type
        // So HasComponent() should be used before it
        return ((ComponentsPoolWrapper<T>*)componentsMap[TYPE_ID(T)])->Storage.Get(id);
    }

    template<class T>
    bool RemoveComponent(EntityID entityID)
    {
        if (isCleared)
            return false;

        auto typeID = TYPE_ID(T);
        EntityID id = EntityIDGetID(entityID);

        ComponentsPoolWrapper<T>* pool;
        if (componentsMap.find(typeID) != componentsMap.end())
        {
            pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];

            if (!pool->Storage.Has(id))
                return false;

            if (pool->System != nullptr)
            {
                auto& component = pool->Storage.Get(id);
                pool->System->OnComponentRemoved(entityID, component);
            }
            pool->Storage.Remove(id);

            return true;
        }

        return false;
    }
};
