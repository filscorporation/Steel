#pragma once

#include "Steel/EntityComponentSystem/EntitiesRegistry.h"

class TypeInfoRegistererBase
{
public:
    virtual ~TypeInfoRegistererBase() = default;;

    virtual void RegisterType(EntitiesRegistry* entitiesRegistry) = 0;
    virtual const TypeInfo* GetTypeInfo() = 0;
};

template<class T>
class TypeInfoRegisterer : TypeInfoRegistererBase
{
public:
    void RegisterType(EntitiesRegistry* entitiesRegistry) override
    {
        // Skip simple types
    }

    const TypeInfo* GetTypeInfo() override
    {
        return T::GetTypeInfo();
    }
};

template<class T>
class ComponentTypeRegisterer : TypeInfoRegistererBase
{
public:
    void RegisterType(EntitiesRegistry* entitiesRegistry) override
    {
        entitiesRegistry->RegisteredType<T>();
    }

    const TypeInfo* GetTypeInfo() override
    {
        return T::GetTypeInfo();
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

    template<class T>
    static void RegisterComponentInStorage()
    {
        if (storage.find(TYPE_ID(T)) != storage.end())
            return;

        storage[TYPE_ID(T)] = (TypeInfoRegistererBase*)new ComponentTypeRegisterer<T>();
    }

    static void Clear();
    static void InitializeTypesFromStorage(EntitiesRegistry* entitiesRegistry);
    static void GetTypeInfos(std::vector<const TypeInfo*>& typeInfos);
    static const TypeInfo* GetTypeInfo(ComponentTypeID typeID);

private:
    static std::unordered_map<ComponentTypeID, TypeInfoRegistererBase*> storage;
};
