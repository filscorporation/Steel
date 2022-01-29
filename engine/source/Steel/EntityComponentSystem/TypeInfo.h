#pragma once

#include <cstdint>

using ComponentTypeID = uint64_t;

class TypeInfo
{
public:
    explicit TypeInfo(const char* className, bool isComponent = true);

    const char* TypeName;
    ComponentTypeID ID;
    const bool IsComponent = true;
};

#define TYPE_ID(m_type) m_type::GetTypeInfo()->ID
#define TYPE_NAME(m_type) m_type::GetTypeInfo()->TypeName
#define TYPE_IS_COMPONENT(m_type) m_type::GetTypeInfo()->IsComponent

#define REGISTER_COMPONENT(typeName) TypeInfoStorage::RegisterComponentInStorage<typeName>()
#define REGISTER_TYPE(typeName) TypeInfoStorage::RegisterTypeInStorage<typeName>()

#define DEFINE_TYPE(typeName)                                   \
public:                                                         \
    static const TypeInfo* GetTypeInfo()                        \
    {                                                           \
        static const TypeInfo typeInfoStatic(#typeName, false); \
        return &typeInfoStatic;                                 \
    }                                                           \
                                                                \
    static void RegisterType();


#define DEFINE_COMPONENT(typeName)                              \
public:                                                         \
    static const TypeInfo* GetTypeInfo()                        \
    {                                                           \
        static const TypeInfo typeInfoStatic(#typeName);        \
        return &typeInfoStatic;                                 \
    }                                                           \
                                                                \
    static void RegisterType();
