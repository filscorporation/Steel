#pragma once

#include <vector>

#include "ScriptingCommon.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/Physics/Collision.h"

struct Collision;

class ScriptComponent : public Component
{
    DEFINE_TYPE(ScriptComponent)

public:
    explicit ScriptComponent(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;

    void AddScript(ScriptObjectHandler* scriptHandler, ScriptTypeInfo* typeInfo);
    bool HasScriptType(ScriptTypeInfo* typeInfo);
    ScriptObjectHandler* GetScriptHandler(ScriptTypeInfo* typeInfo);
    void RemoveScript(ScriptTypeInfo* typeInfo);

    const std::vector<ScriptData>& GetScriptsData() const;
    void SetScriptsData(const std::vector<ScriptData>& scripts);

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

    std::vector<ScriptData> Scripts;
    ScriptEventTypes::ScriptEventType ScriptsMask = (ScriptEventTypes::ScriptEventType)0;

private:
    void TryCallEventMethod(ScriptEventTypes::ScriptEventType eventType, void** params);
    void TryCallCollisionMethod(ScriptEventTypes::ScriptEventType eventType, Collision collision);
};
