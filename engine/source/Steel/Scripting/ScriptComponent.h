#pragma once

#include <vector>

#include "ScriptingCommon.h"
#include "../EntityComponentSystem/Component.h"
#include "../Physics/Collision.h"

struct Collision;

class ScriptComponent : public Component
{
public:
    explicit ScriptComponent(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;

    void AddScript(ScriptPointer scriptPointer, ScriptTypeInfo* typeInfo);
    bool HasScriptType(ScriptTypeInfo* typeInfo);
    ScriptPointer GetScriptPointer(ScriptTypeInfo* typeInfo);
    void RemoveScript(ScriptTypeInfo* typeInfo);

    void OnUpdate();
    void OnCreate();
    void OnDestroy();
    void OnFixedUpdate();
    void OnLateUpdate();
    void OnEnabled();
    void OnDisabled();

    void OnCollisionEnter(Collision collision);
    void OnCollisionStay(Collision collision);
    void OnCollisionExit(Collision collision);

    void OnMouseOver();
    void OnMouseEnter();
    void OnMouseExit();
    void OnMousePressed();
    void OnMouseJustPressed();
    void OnMouseJustReleased();

    void OnMouseOverUI();
    void OnMouseEnterUI();
    void OnMouseExitUI();
    void OnMousePressedUI();
    void OnMouseJustPressedUI();
    void OnMouseJustReleasedUI();

    struct ScriptData
    {
        ScriptPointer Pointer;
        ScriptTypeInfo* TypeInfo;
    };
    std::vector<ScriptData> Scripts;
    ScriptEventTypes::ScriptEventType ScriptsMask = (ScriptEventTypes::ScriptEventType)0;
};

#define CALL_IF_MASK(m_method) \
{ \
    if (!(ScriptsMask & ScriptEventTypes::m_method)) \
        return; \
    for (auto script : Scripts) \
    { \
        if (script.TypeInfo->Mask & ScriptEventTypes::m_method) \
            ScriptingCore::CallMethod(script.Pointer, ScriptingCore::EngineCalls.call##m_method); \
    } \
}

#define CALL_IF_MASK_PARAM(m_method, m_param) \
{ \
    if (!(ScriptsMask & ScriptEventTypes::m_method)) \
        return; \
    for (auto script : Scripts) \
    { \
        if (script.TypeInfo->Mask & ScriptEventTypes::m_method) \
            ScriptingCore::CallMethod(script.Pointer, ScriptingCore::EngineCalls.call##m_method, m_param); \
    } \
}
