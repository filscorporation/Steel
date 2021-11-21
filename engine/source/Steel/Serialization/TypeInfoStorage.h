#pragma once

#include "../EntityComponentSystem/EntitiesRegistry.h"

class TypeInfoRegistererBase
{
public:
    virtual ~TypeInfoRegistererBase() = default;;

    virtual void CheckTypeRegistered(EntitiesRegistry* entitiesRegistry) = 0;
};

template<class T>
class TypeInfoRegisterer : TypeInfoRegistererBase
{
public:
    void CheckTypeRegistered(EntitiesRegistry* entitiesRegistry) override
    {
        entitiesRegistry->CheckTypeRegistered<T>();
    }
};

class TypeInfoStorage
{
public:
    template<class T>
    static void RegisterTypeInStorage()
    {
        if (storage.find(TYPE_ID(T)) != storage.end())
            return;

        storage[TYPE_ID(T)] = (TypeInfoRegistererBase*)new TypeInfoRegisterer<T>();
    }

    static void Clear();
    static bool CheckTypeRegistered(ComponentTypeID typeID, EntitiesRegistry* entitiesRegistry);

private:
    static std::unordered_map<ComponentTypeID, TypeInfoRegistererBase*> storage;
};
