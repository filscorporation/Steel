#pragma once

#include "AttributeInfo.h"

#include <typeindex>
#include <unordered_map>
#include <vector>

using TypeID = std::type_index;

class SerializationManager
{
public:
    static void Init();
    static void Terminate();

    static void RegisterAttribute(TypeID classTypeID, const AttributeInfo& attributeInfo);
    static std::vector<AttributeInfo>& GetAttributes(TypeID classTypeID);

private:
    static void RegisterComponents();

    static std::unordered_map<TypeID, std::vector<AttributeInfo>> _attributesInfo;
};
