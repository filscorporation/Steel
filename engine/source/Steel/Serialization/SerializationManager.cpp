#include "SerializationManager.h"
#include "../Core/Log.h"

std::unordered_map<TypeID, std::vector<AttributeInfo>> SerializationManager::_attributesInfo;

void SerializationManager::Init()
{
    RegisterComponents();

    Log::LogDebug("Serialization manager initialized");
}

void SerializationManager::Terminate()
{
    for (auto& classInfo : _attributesInfo)
    {
        for (auto& info : classInfo.second)
        {
            info.Clear();
        }
    }
    _attributesInfo.clear();
}

void SerializationManager::RegisterAttribute(TypeID classTypeID, const AttributeInfo& attributeInfo)
{
    _attributesInfo[classTypeID].emplace_back(attributeInfo);
}

std::vector<AttributeInfo>& SerializationManager::GetAttributes(TypeID classTypeID)
{
    return _attributesInfo[classTypeID];
}

void SerializationManager::RegisterComponents()
{
    // TODO: all types
}
