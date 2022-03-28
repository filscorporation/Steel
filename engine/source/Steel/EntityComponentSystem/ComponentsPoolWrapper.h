#pragma once

#include "SparseDataSet.h"
#include "RawComponentData.h"

class EntitiesRegistry;

// Stores all components of one type by entityID
template <typename T>
using ComponentsPool = SparseDataSet<T>;

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

    virtual bool HasByID(EntityID id) = 0;
    virtual bool AddByID(EntityID entityID, EntityID id, bool isActive) = 0;
    virtual bool RemoveByID(EntityID id, bool isActive) = 0;

    // Non template iteration methods
    virtual int Size(bool isActive) = 0;
    // Elements count excluding holes (shouldn't be used for iteration)
    virtual int CondensedSize(bool isActive) = 0;
    virtual EntityID GetOwnerByIndex(int index, bool isActive) = 0;

    // Serialization methods
    virtual RawComponentData GetRawByID(EntityID id) = 0;
    virtual RawComponentData AddRawByID(EntityID id, EntityID entityID, bool active) = 0;
    virtual void OnCreateAll() = 0;

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
    ~ComponentsPoolWrapper() override = default;

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

    bool HasByID(EntityID id) override
    {
        return Storage.Has(id) || InactiveStorage.Has(id);
    }

    bool AddByID(EntityID entityID, EntityID id, bool isActive) override
    {
        if (Storage.Has(id) || InactiveStorage.Has(id))
            return true;

        auto& component = isActive ? Storage.Add(id, entityID) : InactiveStorage.Add(id, entityID);
        if (!component.Validate(_entitiesRegistry))
        {
            if (isActive)
                Storage.Remove(id);
            else
                InactiveStorage.Remove(id);
            return false;
        }

        component.SetDefault(_entitiesRegistry);
        component.OnCreated(_entitiesRegistry);

        return true;
    }

    bool RemoveByID(EntityID id, bool isActive) override
    {
        if (!Storage.Has(id) && !InactiveStorage.Has(id))
            return false;

        auto& component = isActive ? Storage.Get(id) : InactiveStorage.Get(id);
        component.OnRemoved(_entitiesRegistry);

        if (isActive)
            Storage.Remove(id);
        else
            InactiveStorage.Remove(id);

        return true;
    }

    int Size(bool isActive) override
    {
        return isActive ? Storage.Size() : InactiveStorage.Size();
    }

    int CondensedSize(bool isActive) override
    {
        return isActive ? Storage.CondensedSize() : InactiveStorage.CondensedSize();
    }

    EntityID GetOwnerByIndex(int index, bool isActive) override
    {
        return isActive ? Storage[index].Owner : InactiveStorage[index].Owner;
    }

    RawComponentData GetRawByID(EntityID id) override
    {
        if (Storage.Has(id))
            return RawComponentData(&(Storage.Get(id)), TYPE_ID(T));
        if (InactiveStorage.Has(id))
            return RawComponentData(&(InactiveStorage.Get(id)), TYPE_ID(T));
        return RawComponentData {};
    }

    RawComponentData AddRawByID(EntityID id, EntityID entityID, bool active) override
    {
        if (active)
        {
            if (Storage.Has(id))
                return RawComponentData {};
            return RawComponentData(&(Storage.Add(id, entityID)), TYPE_ID(T));
        }
        else
        {
            if (InactiveStorage.Has(id))
                return RawComponentData {};
            return RawComponentData(&(InactiveStorage.Add(id, entityID)), TYPE_ID(T));
        }
    }

    void OnCreateAll() override
    {
        for (auto& component : Storage)
        {
            component.OnCreated(_entitiesRegistry);
        }
        for (auto& component : InactiveStorage)
        {
            component.OnCreated(_entitiesRegistry);
        }
    }
};
