#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <unordered_map>

#include "ScriptingCommon.h"
#include "../EntityComponentSystem/Component.h"
#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "../UI/CallbackType.h"

struct CachedData
{
    MonoClass* classTransformation;
    MonoClass* classNameComponent;
    MonoClass* classBoxCollider;
    MonoClass* classCircleCollider;
    MonoClass* classRigidBody;
    MonoClass* classSpriteRenderer;
    MonoClass* classCamera;
    MonoClass* classAudioListener;
    MonoClass* classAudioSource;
    MonoClass* classAnimator;
    MonoClass* classRectTransformation;
    MonoClass* classUIImage;
    MonoClass* classUIButton;
    MonoClass* classUIText;
    MonoClass* classUIInputField;
    MonoClass* classUIClipping;
    MonoClass* classUICheckBox;
    MonoClass* classUITabs;
    MonoClass* classUILayoutGroup;
};

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
    static void CacheDataTypes(MonoImage* image);

    static void FreeScriptHandle(ScriptPointer scriptPointer);

    static Component& AddComponentFromType(EntityID entity, void* type, bool& success);
    static Component& AddComponentFromMonoClass(EntityID entity, MonoClass* monoClass, bool& success);
    static bool HasComponentFromType(EntityID entity, void* type, bool& success);
    static bool HasComponentFromMonoClass(EntityID entity, MonoClass* monoClass);
    static bool RemoveComponentFromType(EntityID entity, void* type, bool& success);
    static bool RemoveComponentFromMonoClass(EntityID entity, MonoClass* monoClass, bool& success);
    static bool ComponentOwnersFromType(void* type, std::vector<EntityID>& result);
    static bool ComponentOwnersFromMonoClass(MonoClass* monoClass, std::vector<EntityID>& result);

    static bool AddScriptComponentFromType(EntityID entity, void* type, ScriptPointer scriptPointer);
    static bool AddScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass, ScriptPointer scriptPointer);
    static bool HasScriptComponentFromType(EntityID entity, void* type);
    static bool HasScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass);
    static ScriptPointer GetScriptComponentFromType(EntityID entity, void* type, bool& success);
    static ScriptPointer GetScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass, bool& success);
    static bool RemoveScriptComponentFromType(EntityID entity, void* type);
    static bool RemoveScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass);
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
    static MonoArray* ToMonoIntPtrArray(const std::vector<intptr_t>& inArray);
    static MonoArray* ToMonoStringArray(const std::vector<std::string>& inArray);
    static MonoArray* ToMonoFloatArray(const std::vector<float>& inArray);
    static MonoArray* ToMonoIntArray(const std::vector<int>& inArray);
    template<typename T> static MonoArray* ToMonoDataTypeArray(const std::vector<T>& inArray, int cachedDataTypeID);

    static void FromMonoUInt32Array(MonoArray* inArray, std::vector<uint32_t>& outArray);
    static void FromMonoStringArray(MonoArray* inArray, std::vector<std::string>& outArray);
    static void FromMonoFloatArray(MonoArray* inArray, std::vector<float>& outArray);
    static void FromMonoIntArray(MonoArray* inArray, std::vector<int>& outArray);
    template<typename T> static void FromMonoDataTypeArray(MonoArray* inArray, std::vector<T>& outArray, int cachedDataTypeID);

    static EngineCallsMethods EngineCalls;
    static EventManagerMethods EventManagerCalls;
    static CoroutinesManagerMethods CoroutinesManagerCalls;

private:
    static CachedData* cachedAPITypes;
    static std::vector<MonoClass*> cachedDataTypes;
    static std::unordered_map<MonoClass*, ScriptTypeInfo*> scriptsInfo;
    static std::unordered_map<ScriptEventTypes::ScriptEventType, MonoMethodDesc*> eventMethodsDescriptions;
    static MonoClass* baseScriptClass;
};

template<typename T>
MonoArray* ScriptingCore::ToMonoDataTypeArray(const std::vector<T>& inArray, int cachedDataTypeID)
{
    MonoArray* outArray = mono_array_new(mono_domain_get(), cachedDataTypes[cachedDataTypeID], inArray.size());

    for (uint32_t i = 0; i < inArray.size(); ++i)
    {
        mono_array_set(outArray, T, i, inArray[i]);
    }

    return outArray;
}

template<typename T>
void ScriptingCore::FromMonoDataTypeArray(MonoArray* inArray, std::vector<T>& outArray, int cachedDataTypeID)
{
    uint32_t length = mono_array_length(inArray);
    outArray.reserve(length);

    for (uint32_t i = 0; i < length; i++)
    {
        outArray.push_back(mono_array_get(inArray, T, i));
    }
}

#define API_CLASS(m_class) mono_class_from_name(image, "Iron", #m_class)
#define CACHE_CLASS(m_class, m_val) cachedAPITypes->class##m_class = m_val;
#define CACHED_CLASS(m_class) cachedAPITypes->class##m_class

#define RETURN_COMPONENT_OWNERS(m_class) \
{ \
    auto components = entitiesRegistry->GetComponentIterator<m_class>(); \
    int size = components.Size(); \
    result.reserve(size); \
    for (int i = 0; i < size; ++i) \
    { \
        if (components[i].IsAlive()) \
            result.push_back(components[i].Owner); \
    } \
    return true; \
}
