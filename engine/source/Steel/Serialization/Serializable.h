#pragma once

#include "AttributeInfo.h"
#include "SerializationManager.h"

class Serializable
{

};

#define CLASS_ID(m_type) std::type_index(typeid(m_type))
#define REGISTER_ATTRIBUTE(className, name, getter, setter, type, flags)                \
SerializationManager::RegisterAttribute                                                 \
(                                                                                       \
    CLASS_ID(className),                                                                \
    AttributeInfo                                                                       \
    (                                                                                   \
        name,                                                                           \
        new AttributeAccessor<className, type>(&className::getter, &className::setter), \
        flags                                                                           \
    )                                                                                   \
)
