#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <unordered_map>

#include "../EntityComponentSystem/Component.h"
#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "ScriptComponent.h"
#include "ScriptComponentSystem.h"

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
    MonoClass* classUIRenderer;
    MonoClass* classUIImage;
    MonoClass* classUIButton;
    MonoClass* classUIText;
};

struct EngineCallsMethods
{
    MonoMethod* freeScriptHandle;

    MonoMethod* callOnCreate;
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
    static void LoadEngineCallsMethods(MonoImage* image);
    static void LoadEventManagerMethods(MonoImage* image);
    static void LoadCoroutinesManagerMethods(MonoImage* image);
    static void RegisterInternalCalls();
    static void CacheAPITypes(MonoImage* image);
    static void CacheDataTypes(MonoImage* image);

    static void FreeScriptHandle(ScriptPointer scriptPointer);

    static Component& AddComponentFromMonoClass(EntityID entity, MonoClass* monoClass, bool& success);
    static bool HasComponentFromMonoClass(EntityID entity, MonoClass* monoClass);
    static bool RemoveComponentFromMonoClass(EntityID entity, MonoClass *monoClass);
    static std::vector<EntityID> ComponentOwnersFromMonoClass(MonoClass* monoClass);
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
    std::vector<EntityID> result; \
    auto components = entitiesRegistry->GetComponentIterator<m_class>(); \
    int size = components.Size(); \
    result.reserve(size); \
    for (int i = 0; i < size; ++i) \
    { \
        if (components[i].IsAlive()) \
            result.push_back(components[i].Owner); \
    } \
    return result; \
}
