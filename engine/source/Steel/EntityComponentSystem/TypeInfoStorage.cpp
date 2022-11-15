#include "TypeInfoStorage.h"

std::unordered_map<ComponentTypeID, TypeInfoRegistererBase*> TypeInfoStorage::storage;

void TypeInfoStorage::Clear()
{
    for (auto pair : storage)
    {
        delete pair.second;
    }

    storage.clear();
}

void TypeInfoStorage::InitializeTypesFromStorage(EntitiesRegistry* entitiesRegistry)
{
    for (auto typePair : storage)
    {
        typePair.second->RegisterType(entitiesRegistry);
    }
}

void TypeInfoStorage::GetTypeInfos(std::vector<const TypeInfo*>& typeInfos)
{
    for (auto typePair : storage)
    {
        typeInfos.push_back(typePair.second->GetTypeInfo());
    }
}

const TypeInfo* TypeInfoStorage::GetTypeInfo(ComponentTypeID typeID)
{
    // TODO: redundant?
    return storage.find(typeID) == storage.end() ? nullptr : storage[typeID]->GetTypeInfo();
}
