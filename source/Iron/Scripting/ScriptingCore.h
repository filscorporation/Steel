#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <unordered_map>
#include "../Scene/Entity.h"

struct CachedData
{
    MonoClass* classTransformation;
    MonoClass* classBoxCollider;
    MonoClass* classRigidBody;
    MonoClass* classSpriteRenderer;
    MonoClass* classCamera;
    MonoClass* classAudioListener;
    MonoClass* classAudioSource;
    MonoClass* classAnimator;
};

struct EngineCallsMethods
{
    MonoMethod* callOnCreate;
    MonoMethod* callOnUpdate;
    MonoMethod* callOnLateUpdate;
    MonoMethod* callOnFixedUpdate;
};

class ScriptingCore
{
public:
    static void Init(MonoImage* image);
    static void Terminate();
    static void LoadEngineCallsMethods(MonoImage* image);
    static void RegisterInternalCalls();
    static void CacheAPITypes(MonoImage* image);

    static Component* AddComponentFromMonoClass(Entity* entity, MonoClass* monoClass);
    static Component* GetComponentFromMonoClass(Entity* entity, MonoClass* monoClass);
    static int64_t RemoveComponentFromMonoClass(Entity *entity, MonoClass *monoClass);
    static MonoMethod* GetMethod(MonoImage* image, const char* methodName);
    static void CallMethod(int64_t scriptID, MonoMethod* method);
    static void FindAndCallEntryPoint(MonoImage* image);

    static EngineCallsMethods EngineCalls;

private:

    static CachedData* cachedData;
};

#define API_CLASS(m_class) mono_class_from_name(image, "Iron", #m_class)
#define CACHE_CLASS(m_class, m_val) cachedData->class##m_class = m_val;
#define CACHED_CLASS(m_class) cachedData->class##m_class
