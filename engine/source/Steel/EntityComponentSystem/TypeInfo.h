#pragma once

#include <cstdint>

using TypeID = uint64_t;

class TypeInfo
{
public:
    explicit TypeInfo(const char* className, bool isComponent = true);

    const char* TypeName;
    TypeID ID;
    const bool IsComponent = true;
};

#define TYPE_ID(m_type) m_type::GetTypeInfoStatic()->ID
#define TYPE_NAME(m_type) m_type::GetTypeInfoStatic()->TypeName
#define TYPE_IS_COMPONENT(m_type) m_type::GetTypeInfoStatic()->IsComponent

#define REGISTER_COMPONENT(typeName) TypeInfoStorage::RegisterComponentInStorage<typeName>()
#define REGISTER_TYPE(typeName) TypeInfoStorage::RegisterTypeInStorage<typeName>()

#define DEFINE_TYPE(typeName)                                   \
public:                                                         \
    static const TypeInfo* GetTypeInfoStatic()                  \
    {                                                           \
        static const TypeInfo typeInfoStatic(#typeName, false); \
        return &typeInfoStatic;                                 \
    }                                                           \
                                                                \
    static void RegisterType();                                 \
                                                                \
    const TypeInfo* GetTypeInfo() override                      \
    {                                                           \
        return GetTypeInfoStatic();                             \
    }


#define DEFINE_COMPONENT(typeName)                              \
public:                                                         \
    static const TypeInfo* GetTypeInfoStatic()                  \
    {                                                           \
        static const TypeInfo typeInfoStatic(#typeName);        \
        return &typeInfoStatic;                                 \
    }                                                           \
                                                                \
    static void RegisterType();                                 \
                                                                \
    const TypeInfo* GetTypeInfo() override                      \
    {                                                           \
        return GetTypeInfoStatic();                             \
    }
