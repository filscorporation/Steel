#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <unordered_map>

#include "../EntityComponentSystem/Component.h"
#include "../EntityComponentSystem/EntitiesRegistry.h"

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

namespace ScriptEventTypes
{
    enum ScriptEventType
    {
        OnUpdate            = 1 << 0,
        OnCreate            = 1 << 1,
        OnDestroy           = 1 << 2,
        OnFixedUpdate       = 1 << 3,
        OnLateUpdate        = 1 << 4,
        OnEnabled           = 1 << 5,
        OnDisabled          = 1 << 6,
        OnCollisionEnter    = 1 << 7,
        OnCollisionStay     = 1 << 8,
        OnCollisionExit     = 1 << 9,
        OnMouseOver         = 1 << 10,
        OnMouseEnter        = 1 << 11,
        OnMouseExit         = 1 << 12,
        OnMousePressed      = 1 << 13,
        OnMouseJustPressed  = 1 << 14,
        OnMouseJustReleased = 1 << 15,
    };

    inline ScriptEventType operator|(ScriptEventType a, ScriptEventType b)
    {
        return static_cast<ScriptEventType>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline ScriptEventType operator&(ScriptEventType a, ScriptEventType b)
    {
        return static_cast<ScriptEventType>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }
}

struct ScriptTypeInfo
{
    ScriptEventTypes::ScriptEventType Mask;
    // Here will be placed some info about fields for serialization
};

using ScriptPointer = intptr_t;

class ScriptComponentSystem;

class ScriptingCore
{
public:
    static void Init(MonoImage* image);
    static void Terminate();
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
    static bool RemoveComponentFromMonoClass(EntityID entity, MonoClass* monoClass);
    static bool ComponentOwnersFromType(void* type, std::vector<EntityID>& result);
    static bool ComponentOwnersFromMonoClass(MonoClass* monoClass, std::vector<EntityID>& result);

    static bool AddScriptComponentFromType(EntityID entity, ScriptPointer scriptPointer, void* type);
    static bool AddScriptComponentFromMonoClass(EntityID entity, ScriptPointer scriptPointer, MonoClass* monoClass);
    static bool HasScriptComponentFromType(EntityID entity, void* type);
    static bool HasScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass);
    static ScriptPointer GetScriptComponentFromType(EntityID entity, void* type, bool& success);
    static ScriptPointer GetScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass, bool& success);
    static bool RemoveScriptComponentFromType(EntityID entity, void* type);
    static bool RemoveScriptComponentFromMonoClass(EntityID entity, MonoClass* monoClass);
    static bool ScriptComponentPointersFromType(void* type, std::vector<ScriptPointer>& result);
    static void ScriptComponentPointersFromMonoClass(MonoClass* monoClass, std::vector<ScriptPointer>& result);
    static ScriptEventTypes::ScriptEventType ScriptGetEventMask(MonoClass* monoClass);

    static MonoMethod* GetMethod(MonoImage* image, const char* methodName);
    static void CallEventMethod(EntityID ownerEntityID, MonoMethod* method);
    static void CallMethod(MonoMethod* method);
    static void CallMethod(ScriptPointer scriptPointer, MonoMethod* method);
    static void CallMethod(ScriptPointer scriptPointer, MonoMethod* method, EntityID param);
    static void FindAndCallEntryPoint(MonoImage* image);
    static MonoArray* ToMonoUInt32Array(const std::vector<uint32_t>& inArray);
    static MonoArray* ToMonoIntPtrArray(const std::vector<intptr_t>& inArray);
    static void FromMonoUInt32Array(MonoArray* inArray, std::vector<uint32_t>& outArray);
    template<typename T> static MonoArray* ToMonoDataTypeArray(const std::vector<T>& inArray, int cachedDataTypeID);

    static EngineCallsMethods EngineCalls;
    static EventManagerMethods EventManagerCalls;
    static CoroutinesManagerMethods CoroutinesManagerCalls;

private:

    static ScriptComponentSystem* scriptComponentSystem;

    static CachedData* cachedAPITypes;
    static std::vector<MonoClass*> cachedDataTypes;
    static std::unordered_map<MonoClass*, ScriptTypeInfo*> scriptsInfo;
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
