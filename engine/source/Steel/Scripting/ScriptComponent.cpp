#include "ScriptComponent.h"
#include "ScriptingCore.h"
#include "ScriptMethodPointer.h"
#include "Steel/Core/Application.h"
#include "Steel/UI/UIEventHandler.h"
#include "Steel/Serialization/AttributesRegistration.h"

#include <cstdint>

void ScriptComponent::RegisterType()
{
    REGISTER_COMPONENT(ScriptComponent);
    REGISTER_SCRIPTS_ATTRIBUTE(ScriptComponent, "", GetScriptsData, SetScriptsData, AttributeFlags::Public, "");
}

void ScriptComponent::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (entitiesRegistry->HasComponent<UIEventHandler>(Owner))
    {
        entitiesRegistry->GetComponent<UIEventHandler>(Owner).EnableNotifyScripts();
    }

    if (!Application::Instance->IsRunning())
        return;

    TryCallEventMethod(ScriptEventTypes::OnCreate, nullptr);
}

void ScriptComponent::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (entitiesRegistry->HasComponent<UIEventHandler>(Owner))
    {
        entitiesRegistry->GetComponent<UIEventHandler>(Owner).DisableNotifyScripts();
    }

    OnDestroy();

    for (auto script : Scripts)
    {
        delete script.ScriptHandler;
    }
}

void ScriptComponent::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    OnEnabled();
}

void ScriptComponent::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    OnDisabled();
}

bool ScriptComponent::HasScriptType(ScriptTypeInfo* typeInfo)
{
    for (auto script : Scripts)
    {
        if (script.TypeInfo == typeInfo)
            return true;
    }

    return false;
}

void ScriptComponent::AddScript(ScriptObjectHandler* scriptHandler, ScriptTypeInfo* typeInfo)
{
    // TODO: rework with scripts being added and removed in edit mode in mind (should store only type, no handle)
    ScriptData data { scriptHandler, typeInfo };
    Scripts.emplace_back(data);
    ScriptsMask = ScriptsMask | typeInfo->Mask;

    if (typeInfo->Mask & ScriptEventTypes::OnCreate && Application::Context()->Scripting)
    {
        ScriptingCore::CallMethod(
                typeInfo->EventMethods[ScriptEventTypes::OnCreate]->Method,
                scriptHandler->GetMonoObject(),
                nullptr
        );
    }
}

void ScriptComponent::RemoveScript(ScriptTypeInfo* typeInfo)
{
    uint32_t i;
    bool found = false;
    for (i = 0; i < Scripts.size(); ++i)
    {
        if (Scripts[i].TypeInfo == typeInfo)
        {
            found = true;
            if (Scripts[i].TypeInfo->Mask & ScriptEventTypes::OnDestroy && Application::Context()->Scripting)
            {
                ScriptingCore::CallMethod(
                        Scripts[i].TypeInfo->EventMethods[ScriptEventTypes::OnDestroy]->Method,
                        Scripts[i].ScriptHandler->GetMonoObject(),
                        nullptr
                );
            }
            delete Scripts[i].ScriptHandler; // This will free gc handle
            break;
        }
    }

    if (!found)
        return;

    Scripts.erase(Scripts.begin() + i);

    ScriptsMask = (ScriptEventTypes::ScriptEventType)0;
    for (auto script : Scripts)
        ScriptsMask = ScriptsMask | script.TypeInfo->Mask;
}

const std::vector<ScriptData>& ScriptComponent::GetScriptsData() const
{
    return Scripts;
}

void ScriptComponent::SetScriptsData(const std::vector<ScriptData>& scripts)
{
    Scripts = scripts;

    ScriptsMask = (ScriptEventTypes::ScriptEventType)0;
    for (auto script : Scripts)
        ScriptsMask = ScriptsMask | script.TypeInfo->Mask;

    for (auto script : Scripts)
    {
        ScriptingCore::SetEntityOwner(script.ScriptHandler->GetMonoObject(), Owner);
    }
}

ScriptObjectHandler* ScriptComponent::GetScriptHandler(ScriptTypeInfo* typeInfo)
{
    for (auto script : Scripts)
    {
        if (script.TypeInfo == typeInfo)
            return script.ScriptHandler;
    }

    return nullptr;
}

void ScriptComponent::OnCreate()
{
    if (!Application::Instance->IsRunning())
        return;

    TryCallEventMethod(ScriptEventTypes::OnCreate, nullptr);
}

void ScriptComponent::OnDestroy()
{
    if (!Application::Instance->IsRunning())
        return;

    TryCallEventMethod(ScriptEventTypes::OnDestroy, nullptr);
}

void ScriptComponent::OnUpdate()
{
    TryCallEventMethod(ScriptEventTypes::OnUpdate, nullptr);
}

void ScriptComponent::OnLateUpdate()
{
    TryCallEventMethod(ScriptEventTypes::OnLateUpdate, nullptr);
}

void ScriptComponent::OnFixedUpdate()
{
    TryCallEventMethod(ScriptEventTypes::OnFixedUpdate, nullptr);
}

void ScriptComponent::OnEnabled()
{
    if (!Application::Instance->IsRunning())
        return;

    TryCallEventMethod(ScriptEventTypes::OnEnabled, nullptr);
}

void ScriptComponent::OnDisabled()
{
    if (!Application::Instance->IsRunning())
        return;

    TryCallEventMethod(ScriptEventTypes::OnDisabled, nullptr);
}

void ScriptComponent::OnCollisionEnter(Collision collision)
{
    TryCallCollisionMethod(ScriptEventTypes::OnCollisionEnter, collision);
}

void ScriptComponent::OnCollisionStay(Collision collision)
{
    TryCallCollisionMethod(ScriptEventTypes::OnCollisionStay, collision);
}

void ScriptComponent::OnCollisionExit(Collision collision)
{
    TryCallCollisionMethod(ScriptEventTypes::OnCollisionExit, collision);
}

void ScriptComponent::OnMouseOver()
{
    TryCallEventMethod(ScriptEventTypes::OnMouseOver, nullptr);
}

void ScriptComponent::OnMouseEnter()
{
    TryCallEventMethod(ScriptEventTypes::OnMouseEnter, nullptr);
}

void ScriptComponent::OnMouseExit()
{
    TryCallEventMethod(ScriptEventTypes::OnMouseExit, nullptr);
}

void ScriptComponent::OnMousePressed()
{
    TryCallEventMethod(ScriptEventTypes::OnMousePressed, nullptr);
}

void ScriptComponent::OnMouseJustPressed()
{
    TryCallEventMethod(ScriptEventTypes::OnMouseJustPressed, nullptr);
}

void ScriptComponent::OnMouseJustReleased()
{
    TryCallEventMethod(ScriptEventTypes::OnMouseJustReleased, nullptr);
}

void ScriptComponent::OnMouseOverUI()
{
    TryCallEventMethod(ScriptEventTypes::OnMouseOverUI, nullptr);
}

void ScriptComponent::OnMouseEnterUI()
{
    TryCallEventMethod(ScriptEventTypes::OnMouseEnterUI, nullptr);
}

void ScriptComponent::OnMouseExitUI()
{
    TryCallEventMethod(ScriptEventTypes::OnMouseExitUI, nullptr);
}

void ScriptComponent::OnMousePressedUI()
{
    TryCallEventMethod(ScriptEventTypes::OnMousePressedUI, nullptr);
}

void ScriptComponent::OnMouseJustPressedUI()
{
    TryCallEventMethod(ScriptEventTypes::OnMouseJustPressedUI, nullptr);
}

void ScriptComponent::OnMouseJustReleasedUI()
{
    TryCallEventMethod(ScriptEventTypes::OnMouseJustReleasedUI, nullptr);
}

void ScriptComponent::TryCallEventMethod(ScriptEventTypes::ScriptEventType eventType, void** params)
{
    if (!Application::Context()->Scripting)
        return;

    if (!(ScriptsMask & eventType))
        return;

    for (auto script : Scripts)
    {
        if (script.TypeInfo->Mask & eventType)
            ScriptingCore::CallMethod(
                    script.TypeInfo->EventMethods[eventType]->Method,
                    script.ScriptHandler->GetMonoObject(),
                    params
            );
    }
}

void ScriptComponent::TryCallCollisionMethod(ScriptEventTypes::ScriptEventType eventType, Collision collision)
{
    void* ctorParams[1];
    MonoObject* entityObject = ScriptingCore::CreateEntityObject(collision.OtherEntity);
    ctorParams[0] = entityObject;
    MonoObject* collisionObject = ScriptingCore::CreateAPIStruct(APIStructs::Collision, ctorParams);

    void* params[1];
    params[0] = collisionObject;
    TryCallEventMethod(eventType, params);
}
