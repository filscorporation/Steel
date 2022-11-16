#pragma once

#include <cstdint>

using TypeID = uint64_t;

class TypeInfo
{
public:
    explicit TypeInfo(const char* className);

    const char* TypeName;
    TypeID ID;
};

#define TYPE_ID(m_type) m_type::GetTypeInfoStatic()->ID
#define TYPE_NAME(m_type) m_type::GetTypeInfoStatic()->TypeName

#define REGISTER_TYPE(typeName) TypeInfoStorage::RegisterTypeInStorage<typeName>()
#define REGISTER_COMPONENT(typeName) TypeInfoStorage::RegisterComponentInStorage<typeName>()

#define DEFINE_TYPE(typeName)                                   \
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
