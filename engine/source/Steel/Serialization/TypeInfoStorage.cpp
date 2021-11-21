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

bool TypeInfoStorage::CheckTypeRegistered(ComponentTypeID typeID, EntitiesRegistry* entitiesRegistry)
{
    if (storage.find(typeID) == storage.end())
        return false;

    storage[typeID]->CheckTypeRegistered(entitiesRegistry);
    return true;
}
