#pragma once

#include "AttributeInfo.h"
#include "AttributeAccessor.h"
#include "IDAttributeAccessor.h"
#include "EnumAttributeAccessor.h"
#include "ResourceAttributeAccessor.h"
#include "SerializationManager.h"
#include "TypeInfoStorage.h"
#include "Steel/Core/TypeInfo.h"

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
    }                                                       \
                                                            \
    static void RegisterType();

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

#define REGISTER_ENUM_ATTRIBUTE(className, name, getter, setter, type, flags)                                   \
SerializationManager::RegisterAttribute                                                                         \
(                                                                                                               \
    TYPE_ID(className),                                                                                         \
    AttributeInfo                                                                                               \
    (                                                                                                           \
        name,                                                                                                   \
        new EnumAttributeAccessor<className, type>(&className::getter, &className::setter),                     \
        flags                                                                                                   \
    )                                                                                                           \
)

#define REGISTER_ID_ATTRIBUTE(className, name, getter, setter, flags)                                           \
SerializationManager::RegisterAttribute                                                                         \
(                                                                                                               \
    TYPE_ID(className),                                                                                         \
    AttributeInfo                                                                                               \
    (                                                                                                           \
        name,                                                                                                   \
        new IDAttributeAccessor<className>(&className::getter, &className::setter),                             \
        flags                                                                                                   \
    )                                                                                                           \
)

#define REGISTER_RESOURCE_ATTRIBUTE(className, name, getter, setter, type, resourceType, flags)                 \
SerializationManager::RegisterAttribute                                                                         \
(                                                                                                               \
    TYPE_ID(className),                                                                                         \
    AttributeInfo                                                                                               \
    (                                                                                                           \
        name,                                                                                                   \
        new ResourceAttributeAccessor<className, type>(&className::getter, &className::setter, resourceType),   \
        flags                                                                                                   \
    )                                                                                                           \
)
