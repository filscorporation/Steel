#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <unordered_map>

#include "ScriptingCommon.h"
#include "SimpleAPITypes.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "Steel/UI/CallbackType.h"

struct EngineCallsMethods
{
    MonoMethod* freeScriptHandle;

    MonoMethod* callOnCreate;
    MonoMethod* callOnDestroy;
    MonoMethod* callOnUpdate;
    MonoMethod* callOnLateUpdate;
    MonoMethod* callOnFixedUpdate;
    MonoMethod* callOnEnabled;
    MonoMethod* callOnDisabled;

    MonoMethod* callOnCollisionEnter;
    MonoMethod* callOnCollisionStay;
    MonoMethod* callOnCollisionExit;

    MonoMethod* callOnMouseOver;
    MonoMethod* callOnMouseEnter;
    MonoMethod* callOnMouseExit;
    MonoMethod* callOnMousePressed;
    MonoMethod* callOnMouseJustPressed;
    MonoMethod* callOnMouseJustReleased;

    MonoMethod* callOnMouseOverUI;
    MonoMethod* callOnMouseEnterUI;
    MonoMethod* callOnMouseExitUI;
    MonoMethod* callOnMousePressedUI;
    MonoMethod* callOnMouseJustPressedUI;
    MonoMethod* callOnMouseJustReleasedUI;
};

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
    static void LoadEventMethodsDescriptions(MonoImage* image);
    static void LoadEngineCallsMethods(MonoImage* image);
    static void LoadEventManagerMethods(MonoImage* image);
    static void LoadCoroutinesManagerMethods(MonoImage* image);
    static void RegisterInternalCalls();
    static void CacheAPITypes(MonoImage* image);
    static void CacheSimpleAPITypes(MonoImage* image);

    static void FreeScriptHandle(ScriptPointer scriptPointer);

    static bool AddComponentFromType(EntityID entityID, void* type);
    static bool AddComponentFromMonoClass(EntityID entityID, MonoClass* monoClass);
    static bool HasComponentFromType(EntityID entityID, void* type);
    static bool HasComponentFromMonoClass(EntityID entityID, MonoClass* monoClass);
    static bool RemoveComponentFromType(EntityID entityID, void* type);
    static bool RemoveComponentFromMonoClass(EntityID entityID, MonoClass* monoClass);
    static bool ComponentOwnersFromType(void* type, std::vector<EntityID>& result);
    static bool ComponentOwnersFromMonoClass(MonoClass* monoClass, std::vector<EntityID>& result);

    static bool AddScriptComponentFromType(EntityID entityID, void* type, ScriptPointer scriptPointer);
    static bool AddScriptComponentFromMonoClass(EntityID entityID, MonoClass* monoClass, ScriptPointer scriptPointer);
    static bool HasScriptComponentFromType(EntityID entityID, void* type);
    static bool HasScriptComponentFromMonoClass(EntityID entityID, MonoClass* monoClass);
    static ScriptPointer GetScriptComponentFromType(EntityID entityID, void* type, bool& success);
    static ScriptPointer GetScriptComponentFromMonoClass(EntityID entityID, MonoClass* monoClass, bool& success);
    static bool RemoveScriptComponentFromType(EntityID entityID, void* type);
    static bool RemoveScriptComponentFromMonoClass(EntityID entityID, MonoClass* monoClass);
    static bool ScriptComponentPointersFromType(void* type, std::vector<ScriptPointer>& result);
    static void ScriptComponentPointersFromMonoClass(MonoClass* monoClass, std::vector<ScriptPointer>& result);
    static MonoClass* TypeToMonoClass(void* type);
    static ScriptTypeInfo* ScriptParseRecursive(MonoClass* monoClass);

    static MonoMethod* GetMethod(MonoImage* image, const char* methodName);
    static void CallEventMethod(EntityID ownerEntityID, CallbackTypes::CallbackType callbackType, MonoMethod* method);
    static void CallMethod(MonoMethod* method);
    static void CallMethod(ScriptPointer scriptPointer, MonoMethod* method);
    static void CallMethod(ScriptPointer scriptPointer, MonoMethod* method, EntityID param);
    static void FindAndCallEntryPoint(MonoImage* image);

    static const char* ToString(MonoString* monoString);

    static MonoArray* ToMonoUInt32Array(const std::vector<uint32_t>& inArray);
    static MonoArray* ToMonoUInt64Array(const std::vector<uint64_t>& inArray);
    static MonoArray* ToMonoIntPtrArray(const std::vector<intptr_t>& inArray);
    static MonoArray* ToMonoStringArray(const std::vector<std::string>& inArray);
    static MonoArray* ToMonoFloatArray(const std::vector<float>& inArray);
    static MonoArray* ToMonoIntArray(const std::vector<int>& inArray);
    template<typename T> static MonoArray* ToMonoSimpleTypeArray(const std::vector<T>& inArray, SimpleAPITypes::SimpleAPIType type);

    static void FromMonoUInt32Array(MonoArray* inArray, std::vector<uint32_t>& outArray);
    static void FromMonoUInt64Array(MonoArray* inArray, std::vector<uint64_t>& outArray);
    static void FromMonoStringArray(MonoArray* inArray, std::vector<std::string>& outArray);
    static void FromMonoFloatArray(MonoArray* inArray, std::vector<float>& outArray);
    static void FromMonoIntArray(MonoArray* inArray, std::vector<int>& outArray);
    template<typename T> static void FromMonoSimpleTypeArray(MonoArray* inArray, std::vector<T>& outArray, SimpleAPITypes::SimpleAPIType type);

    static EngineCallsMethods EngineCalls;
    static EventManagerMethods EventManagerCalls;
    static CoroutinesManagerMethods CoroutinesManagerCalls;

private:
    static std::unordered_map<MonoClass*, const TypeInfo*> cachedAPITypes;
    static std::unordered_map<SimpleAPITypes::SimpleAPIType, MonoClass*> cachedSimpleAPITypes;
    static std::unordered_map<MonoClass*, ScriptTypeInfo*> scriptsInfo;
    static std::unordered_map<ScriptEventTypes::ScriptEventType, MonoMethodDesc*> eventMethodsDescriptions;
    static MonoClass* baseScriptClass;
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

#define CACHE_SIMPLE_TYPE(m_type) cachedSimpleAPITypes[SimpleAPITypes::m_type] = mono_class_from_name(image, "Steel", #m_type)
