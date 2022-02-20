#pragma once

#include "ScriptingCommon.h"
#include "ScriptingDomain.h"
#include "ScriptObjectHandler.h"
#include "SimpleAPITypes.h"
#include "APIStructs.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "Steel/UI/CallbackType.h"

#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <unordered_map>

class ScriptingCore
{
public:
    static bool Init(const char* monoLibPath, const char* monoEtcPath);
    static void Terminate();

    static bool CreateDomain(const char* apiDllPath, const char* scriptsDllPath);
    static void UnloadDomain();
    static bool DomainLoaded();

    static MonoImage* LoadAssemblyImage(const char* filePath);

    static void LoadSpecialTypes(MonoImage* image);
    static void LoadEventMethodsDescriptions(MonoImage* image);
    static void LoadEventManagerMethods(MonoImage* image);
    static void LoadCoroutinesManagerMethods(MonoImage* image);
    static void RegisterInternalCalls();
    static void CacheAPITypes(MonoImage* image);
    static void CacheSimpleAPITypes(MonoImage* image);
    static void CacheAPIStructs(MonoImage* image);

    static MonoObject* AddComponentFromType(EntityID entityID, void* type);
    static bool HasComponentFromType(EntityID entityID, void* type);
    static MonoObject* GetComponentFromType(EntityID entityID, void* type);
    static bool RemoveComponentFromType(EntityID entityID, void* type);
    static void GetComponentsListFromType(void* type, bool includeInactive, MonoObject** result);
    static MonoArray* GetBuiltInComponentsList(MonoClass* monoClass, bool includeInactive);
    static MonoArray* GetCustomComponentsList(MonoClass* monoClass, bool includeInactive);

    static MonoClass* TypeToMonoClass(void* type);
    static MonoClass* GetMonoClassByFullName(const std::string& classNamespace, const std::string& className);
    static bool IsSubclassOfScriptComponent(MonoClass* monoClass);
    static ScriptObjectHandler* CreateManagedInstance(MonoClass* monoClass, void** constructorParams, int paramsCount);
    static MonoObject* CreateUnmanagedInstance(MonoClass* monoClass, void** constructorParams, int paramsCount);
    static MonoObject* CreateAPIStruct(APIStructs::APIStruct structType, void** constructorParams);
    static ScriptTypeInfo* ScriptParseRecursive(MonoClass* monoClass);
    static void SetEntityOwner(MonoObject* monoObject, EntityID entityID);
    static MonoObject* CreateEntityObject(EntityID entityID);
    static bool CanSerializeField(MonoClass* monoClass, MonoClassField* monoClassField);
    static ScriptAttributeAccessorBase* CreateFieldAccessor(MonoClassField* monoClassField, MonoType* monoType);
    template<typename T> static T GetScriptFieldValue(ScriptObjectHandler* scriptHandler, MonoClassField* monoClassField);
    template<typename T> static T GetScriptPtrFieldValue(ScriptObjectHandler* scriptHandler, MonoClassField* monoClassField);
    template<typename T> static void SetScriptFieldValue(ScriptObjectHandler* scriptHandler, MonoClassField* monoClassField, T value);
    template<typename T> static void SetScriptPtrFieldValue(ScriptObjectHandler* scriptHandler, MonoClassField* monoClassField, T value);

    static bool CallMethod(MonoMethod* method, MonoObject* monoObject, void** params);
    static void CallCallbackMethod(EntityID ownerEntityID, CallbackTypes::CallbackType callbackType, MonoMethod* method);
    static void FindAndCallEntryPoint();

    static const char* ToString(MonoString* monoString);
    static MonoString* FromString(const char* normalString);

    static MonoArray* ToMonoUInt32Array(const std::vector<uint32_t>& inArray);
    static MonoArray* ToMonoUInt64Array(const std::vector<uint64_t>& inArray);
    static MonoArray* ToMonoIntPtrArray(const std::vector<intptr_t>& inArray);
    static MonoArray* ToMonoStringArray(const std::vector<std::string>& inArray);
    static MonoArray* ToMonoFloatArray(const std::vector<float>& inArray);
    static MonoArray* ToMonoIntArray(const std::vector<int>& inArray);
    static MonoArray* ToMonoObjectArray(const std::vector<MonoObject*>& inArray);
    template<typename T> static MonoArray* ToMonoSimpleTypeArray(const std::vector<T>& inArray, SimpleAPITypes::SimpleAPIType type);

    static void FromMonoUInt32Array(MonoArray* inArray, std::vector<uint32_t>& outArray);
    static void FromMonoUInt64Array(MonoArray* inArray, std::vector<uint64_t>& outArray);
    static void FromMonoStringArray(MonoArray* inArray, std::vector<std::string>& outArray);
    static void FromMonoFloatArray(MonoArray* inArray, std::vector<float>& outArray);
    static void FromMonoIntArray(MonoArray* inArray, std::vector<int>& outArray);
    static void FromMonoObjectArray(MonoArray* inArray, std::vector<MonoObject*>& outArray);
    template<typename T> static void FromMonoSimpleTypeArray(MonoArray* inArray, std::vector<T>& outArray, SimpleAPITypes::SimpleAPIType type);

    static MonoMethod* GetInvokeCallbacksMethod();
    static MonoMethod* GetDeregisterCallbacksMethod();
    static MonoMethod* GetCoroutinesUpdateMethod();

private:
    static ScriptingDomain* Domain;
};

template<typename T>
MonoArray* ScriptingCore::ToMonoSimpleTypeArray(const std::vector<T>& inArray, SimpleAPITypes::SimpleAPIType type)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), Domain->CachedSimpleAPITypes[type], inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, T, i, inArray[i]);
    }

    return outArray;
}

template<typename T>
void ScriptingCore::FromMonoSimpleTypeArray(MonoArray* inArray, std::vector<T>& outArray, SimpleAPITypes::SimpleAPIType type)
{
    uint32_t length = mono_array_length(inArray);
    outArray.reserve(length);

    for (uint32_t i = 0; i < length; i++)
    {
        outArray.push_back(mono_array_get(inArray, T, i));
    }
}

template<typename T>
T ScriptingCore::GetScriptFieldValue(ScriptObjectHandler* scriptHandler, MonoClassField* monoClassField)
{
    MonoObject* monoObject = scriptHandler->GetMonoObject();
    if (monoObject == nullptr)
    {
        Log::LogError("Can't get mono object from handler");
        T temp;
        return temp;
    }

    T value;
    mono_field_get_value(monoObject, monoClassField, &value);

    return value;
}

template<typename T>
T ScriptingCore::GetScriptPtrFieldValue(ScriptObjectHandler* scriptHandler, MonoClassField* monoClassField)
{
    MonoObject* monoObject = scriptHandler->GetMonoObject();
    if (monoObject == nullptr)
    {
        Log::LogError("Can't get mono object from handler");
        return nullptr;
    }

    T value = nullptr;
    mono_field_get_value(monoObject, monoClassField, &value);

    return value;
}

template<typename T>
void ScriptingCore::SetScriptFieldValue(ScriptObjectHandler* scriptHandler, MonoClassField* monoClassField, T value)
{
    MonoObject* monoObject = scriptHandler->GetMonoObject();
    if (monoObject == nullptr)
    {
        Log::LogError("Can't get mono object from handler");
        return;
    }

    mono_field_set_value(monoObject, monoClassField, &value);
}

template<typename T>
void ScriptingCore::SetScriptPtrFieldValue(ScriptObjectHandler* scriptHandler, MonoClassField* monoClassField, T value)
{
    MonoObject* monoObject = scriptHandler->GetMonoObject();
    if (monoObject == nullptr)
    {
        Log::LogError("Can't get mono object from handler");
        return;
    }

    mono_field_set_value(monoObject, monoClassField, value);
}

#define CACHE_SIMPLE_TYPE(m_type) Domain->CachedSimpleAPITypes[SimpleAPITypes::m_type] = mono_class_from_name(image, "Steel", #m_type)
