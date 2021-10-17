#pragma once

#include <typeinfo>
#include <typeindex>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "Entity.h"
#include "SparseDataSet.h"

#define ENTITY_ID_MASK 0xFFFFFu
#define ENTITY_VERSION_MASK 0xFFFu
#define ENTITY_ID_SHIFT 12u

#define TYPE_ID(m_type) std::type_index(typeid(m_type))
using ComponentTypeID = std::type_index;

// Stores all components of one type by entityID
template <typename T>
using ComponentsPool = SparseDataSet<T>;

class EntitiesRegistry;

// Base class to store components pool
class ComponentsPoolWrapperBase
{
public:
    explicit ComponentsPoolWrapperBase(EntitiesRegistry* entitiesRegistry) { _entitiesRegistry = entitiesRegistry; }
    virtual ~ComponentsPoolWrapperBase() = default;

    virtual void ClearRemoved() = 0;
    virtual bool MoveFromActiveToInactive(EntityID entityID, EntityID id) = 0;
    virtual bool MoveFromInactiveToActive(EntityID entityID, EntityID id) = 0;

    virtual void BeforeDeleteByEntityID(EntityID entityID, EntityID id) = 0;
    virtual void BeforeDeleteAll() = 0;
    virtual void DeleteByEntityID(EntityID entityID, EntityID id) = 0;
    virtual void AfterEntitySetActive(EntityID entityID, EntityID id, bool isActive) = 0;

protected:
    EntitiesRegistry* _entitiesRegistry = nullptr;
};

// Wrapper around ComponentsPool
template <typename T>
class ComponentsPoolWrapper : public ComponentsPoolWrapperBase
{
public:
    ComponentsPool<T> Storage;
    ComponentsPool<T> InactiveStorage;

    explicit ComponentsPoolWrapper(EntitiesRegistry* entitiesRegistry) : ComponentsPoolWrapperBase(entitiesRegistry) { }
    ~ComponentsPoolWrapper() override = default;;

    void ClearRemoved() override
    {
        Storage.Condense();
        InactiveStorage.Condense();
    }

    bool MoveFromActiveToInactive(EntityID entityID, EntityID id) override
    {
        if (!Storage.Has(id))
            return false;

        std::swap(Storage.Get(id), InactiveStorage.Add(id, entityID));
        Storage.Remove(id);

        return true;
    }

    bool MoveFromInactiveToActive(EntityID entityID, EntityID id) override
    {
        if (!InactiveStorage.Has(id))
            return false;

        std::swap(InactiveStorage.Get(id), Storage.Add(id, entityID));
        InactiveStorage.Remove(id);

        return true;
    }

    void BeforeDeleteByEntityID(EntityID entityID, EntityID id) override
    {
        if (Storage.Has(id))
        {
            Storage.Get(id).OnRemoved(_entitiesRegistry);
        }
        if (InactiveStorage.Has(id))
        {
            InactiveStorage.Get(id).OnRemoved(_entitiesRegistry);
        }
    }

    void BeforeDeleteAll() override
    {
        for (auto& component : Storage)
        {
            component.OnRemoved(_entitiesRegistry);
        }
        for (auto& component : InactiveStorage)
        {
            component.OnRemoved(_entitiesRegistry);
        }
    }

    void DeleteByEntityID(EntityID entityID, EntityID id) override
    {
        if (Storage.Has(id))
            Storage.Remove(id);
        if (InactiveStorage.Has(id))
            InactiveStorage.Remove(id);
    }

    void AfterEntitySetActive(EntityID entityID, EntityID id, bool isActive) override
    {
        if (!(isActive && Storage.Has(id) || !isActive && InactiveStorage.Has(id)))
            return;

        if (isActive)
        {
            Storage.Get(id).OnEnabled(_entitiesRegistry);
        }
        else
        {
            InactiveStorage.Get(id).OnDisabled(_entitiesRegistry);
        }
    }
};

// Stores all components pools by Component type ID
using ComponentsMap = std::unordered_map<ComponentTypeID, ComponentsPoolWrapperBase*>;

// Allows iterating over certain component in ACTIVE entities
template <typename T>
struct ComponentIterator
{
public:
    explicit ComponentIterator(ComponentsPoolWrapper<T>* pool, bool active) { _pool = pool;  _active = active; }

    T& operator[] (int index)
    {
        return _active ? _pool->Storage[index] : _pool->InactiveStorage[index];
    }

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

// Allows access to certain component type without using components types hashmap
template <typename T>
class ComponentAccessor;

class EntitiesRegistry
{
public:
    EntitiesRegistry() = default;
    ~EntitiesRegistry()
    {
        isCleared = true;
        CleanAllEntities();
    };

    bool IsCleared() const
    {
        return isCleared;
    }

private:
    std::vector<EntityID> entityIDs;
    std::vector<EntityStates::EntityState> entityStates;

    int freeIDsCount = 0;
    EntityID nextFreeID = 0;

    bool isCleared = false;

public:
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
            entityStates.emplace_back(EntityStates::IsActive | EntityStates::IsActiveSelf);
        }
        else
        {
            freeIDsCount--;
            result = EntityIDCombine(nextFreeID, EntityIDGetVersion(entityIDs[nextFreeID]));
            if (freeIDsCount > 0)
            {
                nextFreeID = EntityIDGetID(entityIDs[nextFreeID]);
                entityIDs[EntityIDGetID(result)] = result;
                entityStates[EntityIDGetID(result)] = EntityStates::IsActive | EntityStates::IsActiveSelf;
            }
        }

        return result;
    }

    bool EntityExists(EntityID entityID)
    {
        if (entityID == NULL_ENTITY)
            return false;
        EntityID id = EntityIDGetID(entityID);
        return id < entityIDs.size() && EntityIDGetVersion(entityIDs[id]) == EntityIDGetVersion(entityID);
    }

    EntityID EntityActual(EntityID id)
    {
        return entityIDs[id];
    }

    void EntitySetActive(EntityID entityID, bool active, bool self)
    {
        if (!EntityExists(entityID))
            return;

        auto id = EntityIDGetID(entityID);
        EntityStates::EntityState state = entityStates[id];

        if (active)
        {
            if (!self && (state & EntityStates::IsActive || !(state & EntityStates::IsActiveSelf))
                || self && (state & EntityStates::IsActiveSelf))
                return;

            for (auto pool : componentsMap)
            {
                pool.second->MoveFromInactiveToActive(entityID, id);
            }

            entityStates[id] = entityStates[id] | EntityStates::IsActive;
            if (self)
                entityStates[id] = entityStates[id] | EntityStates::IsActiveSelf;
        }
        else
        {
            if (!self && !(state & EntityStates::IsActive) || !(state & EntityStates::IsActiveSelf))
                return;

            for (auto pool : componentsMap)
            {
                pool.second->MoveFromActiveToInactive(entityID, id);
            }

            entityStates[id] = entityStates[id] & ~EntityStates::IsActive;
            if (self)
                entityStates[id] = entityStates[id] & ~EntityStates::IsActiveSelf;
        }

        for (auto pool : componentsMap)
        {
            pool.second->AfterEntitySetActive(entityID, id, active);
        }
    }

    EntityStates::EntityState EntityGetState(EntityID entityID)
    {
        return EntityExists(entityID) ? entityStates[EntityIDGetID(entityID)] : (EntityStates::EntityState)0;
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
        for (auto poolPair : componentsMap)
        {
            poolPair.second->BeforeDeleteAll();
        }
        for (auto poolPair : componentsMap)
        {
            delete poolPair.second;
        }
        entityIDs.clear();
        entityStates.clear();
        nextFreeID = 0;
        freeIDsCount = 0;
    }

private:
    ComponentsMap componentsMap;

public:
    template <typename T>
    int32_t GetComponentsCount()
    {
        auto typeID = TYPE_ID(T);

        if (componentsMap.find(typeID) == componentsMap.end())
        {
            return 0;
        }

        auto pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];
        return pool->Storage.Size() + pool->InactiveStorage.Size();
    }

    template <typename T>
    ComponentIterator<T> GetComponentIterator(bool active = true)
    {
        auto typeID = TYPE_ID(T);

        if (componentsMap.find(typeID) == componentsMap.end())
        {
            return ComponentIterator<T>(nullptr, active);
        }

        return ComponentIterator<T>((ComponentsPoolWrapper<T>*)componentsMap[typeID], active);
    }

    template <typename T>
    ComponentAccessor<T> GetComponentAccessor()
    {
        auto typeID = TYPE_ID(T);

        if (componentsMap.find(typeID) == componentsMap.end())
        {
            return ComponentAccessor<T>(nullptr);
        }

        return ComponentAccessor<T>((ComponentsPoolWrapper<T>*)componentsMap[typeID]);
    }

    // Sorts all components of ACTIVE entities of some type
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
        for (uint32_t i = 0; i < pool->Storage.size; ++i)
        {
            pool->Storage.dense[i] = EntityIDGetID(pool->Storage.data[i].Owner);
            pool->Storage.sparse[pool->Storage.dense[i]] = i;
        }

        return true;
    }

    // Apply sorting order from source component type to target for ACTIVE entities
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
        for (uint32_t i = 0; i < sourcePool->Storage.size; ++i)
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
            pool = new ComponentsPoolWrapper<T>(this);
            componentsMap[typeID] = pool;
        }
        else
        {
            pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];
        }

        if (pool->Storage.Has(id))
            return pool->Storage.Get(id);

        if (pool->InactiveStorage.Has(id))
            return pool->InactiveStorage.Get(id);

        auto& component = entityStates[id] & EntityStates::IsActive
                ? pool->Storage.Add(id, entityID)
                : pool->InactiveStorage.Add(id, entityID);
        if (!component.Validate(this))
        {
            if (entityStates[id] & EntityStates::IsActive)
                pool->Storage.Remove(id);
            else
                pool->InactiveStorage.Remove(id);
        }
        else
        {
            component.OnCreated(this);
        }

        return component;
    }

    template<class T>
    bool HasComponent(EntityID entityID)
    {
        auto typeID = TYPE_ID(T);
        EntityID id = EntityIDGetID(entityID);

        ComponentsPoolWrapper<T>* pool;
        if (componentsMap.find(typeID) == componentsMap.end())
            return false;
        else
            pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];

        return pool->Storage.Has(id) || pool->InactiveStorage.Has(id);
    }

    template<class T>
    T& GetComponent(EntityID entityID)
    {
        EntityID id = EntityIDGetID(entityID);

        // Out of all methods, "get" doesn't check if there is component for the reason of return type
        // So HasComponent() should be used before it
        return entityStates[id] & EntityStates::IsActive
            ? ((ComponentsPoolWrapper<T>*)componentsMap[TYPE_ID(T)])->Storage.Get(id)
            : ((ComponentsPoolWrapper<T>*)componentsMap[TYPE_ID(T)])->InactiveStorage.Get(id);
    }

    template<class T>
    bool RemoveComponent(EntityID entityID)
    {
        auto typeID = TYPE_ID(T);
        EntityID id = EntityIDGetID(entityID);

        ComponentsPoolWrapper<T>* pool;
        if (componentsMap.find(typeID) != componentsMap.end())
        {
            pool = (ComponentsPoolWrapper<T>*)componentsMap[typeID];

            if (!pool->Storage.Has(id) && !pool->InactiveStorage.Has(id))
                return false;

            bool active = entityStates[id] & EntityStates::IsActive;
            auto& component = active ? pool->Storage.Get(id) : pool->InactiveStorage.Get(id);
            component.OnRemoved(this);

            if (active)
                pool->Storage.Remove(id);
            else
                pool->InactiveStorage.Remove(id);

            return true;
        }

        return false;
    }

    void ClearRemoved()
    {
        for (auto component : componentsMap)
        {
            component.second->ClearRemoved();
        }
    }

    template<typename T>
    void ClearRemoved()
    {
        auto typeID = TYPE_ID(T);
        if (componentsMap.find(typeID) != componentsMap.end())
        {
            ((ComponentsPoolWrapper<T>*)componentsMap[typeID])->ClearRemoved();
        }
    }
};

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
        EntityID id = EntitiesRegistry::EntityIDGetID(entityID);
        return _componentsPool->Storage.Has(id);
    }

    T& Get(EntityID entityID) const
    {
        EntityID id = EntitiesRegistry::EntityIDGetID(entityID);
        return _componentsPool->Storage.Get(id);
    }

    bool HasInactive(EntityID entityID) const
    {
        if (_componentsPool == nullptr)
            return false;
        EntityID id = EntitiesRegistry::EntityIDGetID(entityID);
        return _componentsPool->InactiveStorage.Has(id);
    }

    T& GetInactive(EntityID entityID) const
    {
        EntityID id = EntitiesRegistry::EntityIDGetID(entityID);
        return _componentsPool->InactiveStorage.Get(id);
    }

private:
    ComponentsPoolWrapper<T>* _componentsPool;
};
