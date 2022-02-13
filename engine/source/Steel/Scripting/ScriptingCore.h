#pragma once

#include "ScriptingCommon.h"
#include "ScriptPointer.h"
#include "SimpleAPITypes.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "Steel/UI/CallbackType.h"
#include "APIStructs.h"

#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <unordered_map>

struct EventManagerMethods
{
    MonoMethod* callInvokeCallbacks;
    MonoMethod* callDeregisterCallbacks;
};

struct CoroutinesManagerMethods
{
    MonoMethod* callUpdate;
};

class ScriptingCore
{
public:
    static void Init(MonoImage* image);
    static void Terminate();
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
    static void ComponentOwnersFromType(void* type, MonoObject** result);

    static MonoClass* TypeToMonoClass(void* type);
    static bool IsSubclassOfScriptComponent(MonoClass* monoClass);
    static ScriptPointer* CreateManagedInstance(MonoClass* monoClass, void** constructorParams, int paramsCount);
    static MonoObject* CreateUnmanagedInstance(MonoClass* monoClass, void** constructorParams, int paramsCount);
    static ScriptPointer* CreateAPIStruct(APIStructs::APIStruct structType, void** constructorParams);
    static ScriptTypeInfo* ScriptParseRecursive(MonoClass* monoClass);
    static void SetEntityOwner(MonoObject* monoObject, EntityID entityID);
    static bool CanSerializeField(MonoClass* monoClass, MonoClassField* monoClassField);
    static ScriptAttributeAccessorBase* CreateFieldAccessor(MonoClassField* monoClassField, MonoType* monoType);
    template<typename T> static T GetScriptingFieldValue(ScriptPointer* scriptPointer, MonoClassField* monoClassField);
    template<typename T> static void SetScriptingFieldValue(ScriptPointer* scriptPointer, MonoClassField* monoClassField, T value);

    static bool CallMethod(MonoMethod* method, MonoObject* monoObject, void** params);
    static void CallEventMethod(EntityID ownerEntityID, CallbackTypes::CallbackType callbackType, MonoMethod* method);
    static void FindAndCallEntryPoint(MonoImage* image);

    static const char* ToString(MonoString* monoString);

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

    static EventManagerMethods EventManagerCalls;
    static CoroutinesManagerMethods CoroutinesManagerCalls;

private:
    static std::unordered_map<MonoClass*, const TypeInfo*> cachedAPITypes;
    static std::unordered_map<SimpleAPITypes::SimpleAPIType, MonoClass*> cachedSimpleAPITypes;
    static std::unordered_map<APIStructs::APIStruct, std::pair<MonoClass*, MonoMethod*>> cachedAPIStructs;
    static std::unordered_map<MonoClass*, ScriptTypeInfo*> scriptsInfo;
    static std::unordered_map<ScriptEventTypes::ScriptEventType, MonoMethodDesc*> eventMethodsDescriptions;
    static MonoClass* baseScriptClass;
    static MonoClass* serializableAttributeClass;
    static MonoClass* componentClass;
    static MonoClass* entityClass;
    static MonoMethod* entityConstructorMethod;
    static MonoProperty* componentEntityProperty;
};

template<typename T>
MonoArray* ScriptingCore::ToMonoSimpleTypeArray(const std::vector<T>& inArray, SimpleAPITypes::SimpleAPIType type)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), cachedSimpleAPITypes[type], inArray.size());

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
T ScriptingCore::GetScriptingFieldValue(ScriptPointer* scriptPointer, MonoClassField* monoClassField)
{
    MonoObject* monoObject = scriptPointer->GetMonoObject();
    if (monoObject == nullptr)
    {
        Log::LogError("Can't get mono object from pointer");
        T temp;
        return temp;
    }

    T value;
    mono_field_get_value(monoObject, monoClassField, &value);

    return value;
}

template<typename T>
void ScriptingCore::SetScriptingFieldValue(ScriptPointer* scriptPointer, MonoClassField* monoClassField, T value)
{
    MonoObject* monoObject = scriptPointer->GetMonoObject();
    if (monoObject == nullptr)
    {
        Log::LogError("Can't get mono object from pointer");
        return;
    }

    mono_field_set_value(monoObject, monoClassField, &value);
}

#define CACHE_SIMPLE_TYPE(m_type) cachedSimpleAPITypes[SimpleAPITypes::m_type] = mono_class_from_name(image, "Steel", #m_type)
