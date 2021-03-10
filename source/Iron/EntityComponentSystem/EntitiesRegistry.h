#pragma once

#include <typeinfo>
#include <typeindex>
#include <vector>
#include <unordered_map>

#include "Entity.h"

#define TYPE_ID(m_type) std::type_index(typeid(m_type))
using ComponentTypeID = std::type_index;

// Stores all components of one type by entityID
template <typename T>
using ComponentsPool = std::unordered_map<EntityID, T>; // TODO: replace with sparse set

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
    ComponentsPool<T> Value;

    void DeleteByEntityID(EntityID entityID) override
    {
        if (Value.find(entityID) != Value.end())
            Value.erase(entityID);
    }
};

// Stores all components pools by Component type ID
using ComponentsMap = std::unordered_map<ComponentTypeID, ComponentsPoolWrapperBase*>;

// TODO
template <typename T>
struct ComponentIterator
{
public:
    explicit ComponentIterator(ComponentsPoolWrapper<T>* pool) { _pool = pool; }

    using iterator = typename std::unordered_map<EntityID, T>::iterator;
    iterator begin() { return _pool->Value.begin(); }
    iterator end() { return _pool->Value.end(); }

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
        // TODO
        return true;
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

            if (pool->Value.find(entityID) != pool->Value.end())
                return pool->Value.at(entityID);
        }

        pool->Value.emplace(entityID, entityID);

        return pool->Value.at(entityID);
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

        return pool->Value.find(entityID) != pool->Value.end();
    }

    template<class T>
    T& GetComponent(EntityID entityID)
    {
        // Out of all methods, "get" doesn't check if there is component for the reason of return type
        // So HasComponent() should be used before it
        return ((ComponentsPoolWrapper<T>*)componentsMap[TYPE_ID(T)])->Value.at(entityID);
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

            if (pool->Value.find(entityID) != pool->Value.end())
            {
                pool->Value.erase(entityID);
                return true;
            }
        }

        return false;
    }
};
