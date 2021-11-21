#pragma once

#include "AttributeInfo.h"
#include "SerializationManager.h"
#include "TypeInfoStorage.h"
#include "../Core/TypeInfo.h"

class Serializable
{

};

#define REGISTER_TYPE(typeName) TypeInfoStorage::RegisterTypeInStorage<typeName>()

#define COMPONENT(typeName)                                 \
public:                                                     \
    static const TypeInfo* GetTypeInfo()                    \
    {                                                       \
        static const TypeInfo typeInfoStatic(#typeName);    \
        return &typeInfoStatic;                             \
    }

#define REGISTER_ATTRIBUTE(className, name, getter, setter, type, flags)                                        \
SerializationManager::RegisterAttribute                                                                         \
(                                                                                                               \
    TYPE_ID(className),                                                                                         \
    AttributeInfo                                                                                               \
    (                                                                                                           \
        name,                                                                                                   \
        new AttributeAccessor<className, type, AttributeTypedef<type>>(&className::getter, &className::setter), \
        flags                                                                                                   \
    )                                                                                                           \
)
