#pragma once

#include "ComponentAccessor.h"
#include "ComponentIterator.h"
#include "ComponentsPoolWrapper.h"
#include "Entity.h"
#include "EntitiesIterator.h"
#include "SparseDataSet.h"
#include "TypeInfo.h"
#include "RawComponentData.h"
#include "Steel/Core/Log.h"

#include <vector>
#include <unordered_map>
#include <algorithm>

#define DEBUG_GET
#ifdef DEBUG_GET
#include <iostream>
#endif

class TypeInfo;

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

    // Stores all components pools by Component type ID
    std::unordered_map<TypeID, ComponentsPoolWrapperBase*> componentsMap;

    int freeIDsCount = 0;
    EntityID nextFreeID = 0;

    bool isCleared = false;

public:
    EntityID CreateNewEntity()
    {
        EntityID result;
        if (freeIDsCount == 0)
        {
            result = Entity::EntityIDCombine(entityIDs.size(), 1);
            entityIDs.emplace_back(result);
            entityStates.emplace_back(EntityStates::IsActive | EntityStates::IsActiveSelf);
        }
        else
        {
            freeIDsCount--;
            result = Entity::EntityIDCombine(nextFreeID, Entity::EntityIDGetVersion(entityIDs[nextFreeID]));
            if (freeIDsCount > 0)
            {
                nextFreeID = Entity::EntityIDGetID(entityIDs[nextFreeID]);
                entityIDs[Entity::EntityIDGetID(result)] = result;
                entityStates[Entity::EntityIDGetID(result)] = EntityStates::IsActive | EntityStates::IsActiveSelf;
            }
        }

        return result;
    }

    bool EntityExists(EntityID entityID)
    {
        if (entityID == NULL_ENTITY)
            return false;
        EntityID id = Entity::EntityIDGetID(entityID);
        return id < entityIDs.size() && Entity::EntityIDGetVersion(entityIDs[id]) == Entity::EntityIDGetVersion(entityID);
    }

    void GetAllComponentsForEntity(EntityID entityID, std::vector<RawComponentData>& componentsData)
    {
        auto id = Entity::EntityIDGetID(entityID);
        for (auto pool : componentsMap)
        {
            if (pool.second->HasByID(id))
            {
                componentsData.emplace_back(pool.second->GetRawByID(id));
            }
        }
    }

    template<class T>
    void RegisteredType()
    {
        auto typeID = TYPE_ID(T);
        if (componentsMap.find(typeID) != componentsMap.end())
            return;

        componentsMap[typeID] = new ComponentsPoolWrapper<T>(this);
    }

    void EntitySetActive(EntityID entityID, bool active, bool self)
    {
        if (!EntityExists(entityID))
            return;

        auto id = Entity::EntityIDGetID(entityID);
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
        return EntityExists(entityID) ? entityStates[Entity::EntityIDGetID(entityID)] : (EntityStates::EntityState)0;
    }

    void DeleteEntity(EntityID entityID)
    {
        if (!EntityExists(entityID))
            return;

        // Increase this entityID version by one
        entityIDs[Entity::EntityIDGetID(entityID)] = Entity::EntityIDIncrementVersion(entityID);
        if (freeIDsCount != 0)
        {
            // If there are any free ids already, create link from current deleted id to previously deleted
            entityIDs[Entity::EntityIDGetID(entityID)] = Entity::EntityIDCombine(nextFreeID, Entity::EntityIDGetVersion(entityIDs[Entity::EntityIDGetID(entityID)]));
        }
        // Next free id is current deleted
        nextFreeID = Entity::EntityIDGetID(entityID);
        freeIDsCount++;

        for (auto component : componentsMap)
        {
            component.second->BeforeDeleteByEntityID(entityID, Entity::EntityIDGetID(entityID));
        }
        for (auto component : componentsMap)
        {
            component.second->DeleteByEntityID(entityID, Entity::EntityIDGetID(entityID));
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
        componentsMap.clear();
        entityIDs.clear();
        entityStates.clear();
        nextFreeID = 0;
        freeIDsCount = 0;
    }

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

    EntitiesIterator GetEntitiesIterator(TypeID typeID, bool active = true)
    {
        if (componentsMap.find(typeID) == componentsMap.end())
        {
            return EntitiesIterator(nullptr, active);
        }

        return EntitiesIterator(componentsMap[typeID], active);
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
            pool->Storage.dense[i] = Entity::EntityIDGetID(pool->Storage.data[i].Owner);
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
            EntityID id = Entity::EntityIDGetID(sourcePool->Storage.data[i].Owner);
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
        AddComponent(entityID, TYPE_ID(T));

        return GetComponent<T>(entityID);
    }

    bool AddComponent(EntityID entityID, TypeID typeID)
    {
        if (componentsMap.find(typeID) == componentsMap.end())
        {
            Log::LogError("Error adding component: type {0} is not registered", typeID);
            return false;
        }

        EntityID id = Entity::EntityIDGetID(entityID);

        return componentsMap[typeID]->AddByID(entityID, id, entityStates[id] & EntityStates::IsActive);
    }

    template<class T>
    bool HasComponent(EntityID entityID)
    {
        return HasComponent(entityID, TYPE_ID(T));
    }

    bool HasComponent(EntityID entityID, TypeID typeID)
    {
        EntityID id = Entity::EntityIDGetID(entityID);

        if (componentsMap.find(typeID) == componentsMap.end())
            return false;

        return componentsMap[typeID]->HasByID(id);
    }

    template<class T>
    T& GetComponent(EntityID entityID)
    {
        EntityID id = Entity::EntityIDGetID(entityID);

#ifdef DEBUG_GET
        if (!HasComponent<T>(entityID))
        {
            Log::LogError("Get component on non existing type {0} will lead to undefined behavior", TYPE_NAME(T));
        }
#endif

        // Out of all methods, "get" doesn't check if there is component for the reason of return type
        // So HasComponent() should be used before it
        return entityStates[id] & EntityStates::IsActive
            ? ((ComponentsPoolWrapper<T>*)componentsMap[TYPE_ID(T)])->Storage.Get(id)
            : ((ComponentsPoolWrapper<T>*)componentsMap[TYPE_ID(T)])->InactiveStorage.Get(id);
    }

    template<class T>
    bool RemoveComponent(EntityID entityID)
    {
        return RemoveComponent(entityID, TYPE_ID(T));
    }

    bool RemoveComponent(EntityID entityID, TypeID typeID)
    {
        EntityID id = Entity::EntityIDGetID(entityID);

        if (componentsMap.find(typeID) == componentsMap.end())
            return false;

        return componentsMap[typeID]->RemoveByID(id, entityStates[id] & EntityStates::IsActive);
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

    // Serialization methods

    bool RestoreEntityState(EntityID entityID, EntityStates::EntityState entityState)
    {
        if (!EntityExists(entityID))
            return false;

        entityStates[Entity::EntityIDGetID(entityID)] = entityState;
        return true;
    }

    RawComponentData RestoreComponent(TypeID typeID, EntityID entityID)
    {
        EntityID id = Entity::EntityIDGetID(entityID);
        if (!EntityExists(entityID))
            return RawComponentData {};

        if (componentsMap.find(typeID) == componentsMap.end())
            return RawComponentData {};

        return componentsMap[typeID]->AddRawByID(id, entityID, EntityGetState(entityID) & EntityStates::IsActive);
    }

    void OnCreateAll()
    {
        for (auto component : componentsMap)
        {
            component.second->OnCreateAll();
        }
    }
};
