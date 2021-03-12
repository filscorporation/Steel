#pragma once

#include <typeinfo>
#include <typeindex>
#include <vector>
#include <unordered_map>

#include "Entity.h"
#include "SparseSet.h"
#include "ComponentSystem.h"

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
    virtual void DeleteByEntityID(EntityID entityID) = 0;
};

// Wrapper around ComponentsPool
template <typename T>
class ComponentsPoolWrapper : public ComponentsPoolWrapperBase
{
public:
    ComponentsPool<T> Storage;
    ComponentSystem<T>* System = nullptr;

    void DeleteByEntityID(EntityID entityID) override
    {
        if (System != nullptr)
        {
            auto& component = Storage.Get(entityID);
            System->OnComponentRemoved(entityID, component);
        }
        Storage.Remove(entityID);
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
    std::vector<bool> freeEntityIDs; // TODO: maybe not the best solution
    EntityID nextFreeEntityID = 0;
    bool isCleared = false;

public:
    EntityID CreateNewEntity()
    {
        if (nextFreeEntityID == freeEntityIDs.size())
        {
            freeEntityIDs.push_back(false);
            return nextFreeEntityID++;
        }

        EntityID result = nextFreeEntityID;
        freeEntityIDs[nextFreeEntityID] = false;
        nextFreeEntityID++;
        while (nextFreeEntityID < freeEntityIDs.size())
        {
            if (freeEntityIDs[nextFreeEntityID])
                break;
            nextFreeEntityID++;
        }

        return result;
    }

    bool EntityExists(EntityID entityID)
    {
        return !freeEntityIDs[entityID];
    }

    void DeleteEntity(EntityID entityID)
    {
        freeEntityIDs[entityID] = true;
        nextFreeEntityID = nextFreeEntityID < entityID ? nextFreeEntityID : entityID;

        for (auto component : componentsMap)
        {
            component.second->DeleteByEntityID(entityID);
        }
    }

    void CleanAllEntities()
    {
        freeEntityIDs.clear();
        nextFreeEntityID = 0;
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

    template<class T>
    T& AddComponent(EntityID entityID)
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

            if (pool->Storage.Has(entityID))
                return pool->Storage.Get(entityID);
        }

        if (pool->Storage.Has(entityID))
            return pool->Storage.Get(entityID);

        auto& component = pool->Storage.Add(entityID);
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

        ComponentsPoolWrapper<T>* pool;
        if (componentsMap.find(typeID) == componentsMap.end())
            return false;
        else
            pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];

        return pool->Storage.Has(entityID);
    }

    template<class T>
    T& GetComponent(EntityID entityID)
    {
        // Out of all methods, "get" doesn't check if there is component for the reason of return type
        // So HasComponent() should be used before it
        return ((ComponentsPoolWrapper<T>*)componentsMap[TYPE_ID(T)])->Storage.Get(entityID);
    }

    template<class T>
    bool RemoveComponent(EntityID entityID)
    {
        if (isCleared)
            return false;

        auto typeID = TYPE_ID(T);

        ComponentsPoolWrapper<T>* pool;
        if (componentsMap.find(typeID) != componentsMap.end())
        {
            pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];

            if (!pool->Storage.Has(entityID))
                return false;

            if (pool->System != nullptr)
            {
                auto& component = pool->Storage.Get(entityID);
                pool->System->OnComponentRemoved(entityID, component);
            }
            pool->Storage.Remove(entityID);

            return true;
        }

        return false;
    }
};
