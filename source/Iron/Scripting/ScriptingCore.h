#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <unordered_map>

#include "../EntityComponentSystem/Component.h"
#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "ScriptComponent.h"

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
};

struct EngineCallsMethods
{
    MonoMethod* callOnCreate;
    MonoMethod* callOnUpdate;
    MonoMethod* callOnLateUpdate;
    MonoMethod* callOnFixedUpdate;

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

class ScriptingCore
{
public:
    static void Init(MonoImage* image);
    static void Terminate();
    static void LoadEngineCallsMethods(MonoImage* image);
    static void RegisterInternalCalls();
    static void CacheAPITypes(MonoImage* image);

    static Component& AddComponentFromMonoClass(EntityID entity, MonoClass* monoClass, bool& success);
    static bool HasComponentFromMonoClass(EntityID entity, MonoClass* monoClass);
    static bool RemoveComponentFromMonoClass(EntityID entity, MonoClass *monoClass);
    static std::vector<EntityID> ComponentOwnersFromMonoClass(MonoClass* monoClass);
    static MonoMethod* GetMethod(MonoImage* image, const char* methodName);
    static void CallMethod(ScriptPointer scriptPointer, MonoMethod* method);
    static void CallMethod(ScriptPointer scriptPointer, MonoMethod* method, EntityID param);
    static void FindAndCallEntryPoint(MonoImage* image);
    static MonoArray* ToMonoUInt32Array(const std::vector<uint32_t>& inArray);
    static MonoArray* ToMonoIntPtrArray(const std::vector<intptr_t>& inArray);

    static EngineCallsMethods EngineCalls;

private:

    static CachedData* cachedData;
};

#define API_CLASS(m_class) mono_class_from_name(image, "Iron", #m_class)
#define CACHE_CLASS(m_class, m_val) cachedData->class##m_class = m_val;
#define CACHED_CLASS(m_class) cachedData->class##m_class

#define RETURN_COMPONENT_OWNERS(m_class) \
{ \
    std::vector<EntityID> result; \
    auto components = entitiesRegistry->GetComponentIterator<m_class>(); \
    result.reserve(components.Size()); \
    for (int i = 0; i < components.Size(); ++i) \
    { \
        result.push_back(components[i].Owner); \
    } \
    return result; \
}
