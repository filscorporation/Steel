#pragma once

#include <vector>
#include "../EntityComponentSystem/Component.h"
#include "../Physics/Collision.h"
#include "ScriptingCore.h"

struct Collision;

class ScriptComponent : public Component
{
public:
    explicit ScriptComponent(EntityID ownerEntityID) : Component(ownerEntityID) { }

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
    void OnCollisionExit(Collision collision);

    void OnMouseOver();
    void OnMouseEnter();
    void OnMouseExit();
    void OnMousePressed();
    void OnMouseJustPressed();
    void OnMouseJustReleased();

    struct ScriptData
    {
        ScriptPointer ScriptPointer;
        ScriptTypeInfo* TypeInfo;
    };
    std::vector<ScriptData> Scripts;
    ScriptEventTypes::ScriptEventType ScriptsMask = (ScriptEventTypes::ScriptEventType)0;

private:
    bool hasCollision = false;
    Collision collisionStay;
};

#define CALL_IF_MASK(m_method) \
{ \
    if (!(ScriptsMask & ScriptEventTypes::m_method)) \
        return; \
    for (auto script : Scripts) \
    { \
        if (script.TypeInfo->Mask & ScriptEventTypes::m_method) \
            ScriptingCore::CallMethod(script.ScriptPointer, ScriptingCore::EngineCalls.call##m_method); \
    } \
}
