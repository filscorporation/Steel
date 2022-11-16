#include "TypeInfoStorage.h"

std::unordered_map<TypeID, TypeInfoRegistererBase*> TypeInfoStorage::storage;

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
