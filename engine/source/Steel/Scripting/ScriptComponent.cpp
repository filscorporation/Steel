#include "ScriptComponent.h"
#include "ScriptingCore.h"
#include "ScriptMethodPointer.h"
#include "Steel/Core/Application.h"
#include "Steel/UI/UIEventHandler.h"

#include <cstdint>

void ScriptComponent::RegisterType()
{
    REGISTER_COMPONENT(ScriptComponent);
    REGISTER_SCRIPTS_ATTRIBUTE(ScriptComponent, "scripts", GetScriptsData, SetScriptsData, AttributeFlags::Public);
}

void ScriptComponent::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    if (entitiesRegistry->HasComponent<UIEventHandler>(Owner))
    {
        entitiesRegistry->GetComponent<UIEventHandler>(Owner).EnableNotifyScripts();
    }
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
        delete script.Pointer;
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

void ScriptComponent::AddScript(ScriptPointer* scriptPointer, ScriptTypeInfo* typeInfo)
{
    // TODO: rework with scripts being added and removed in edit mode in mind (should store only type, no handle)
    ScriptData data { scriptPointer, typeInfo };
    Scripts.emplace_back(data);
    ScriptsMask = ScriptsMask | typeInfo->Mask;

    if (typeInfo->Mask & ScriptEventTypes::OnCreate)
    {
        ScriptingCore::CallMethod(
                typeInfo->EventMethods[ScriptEventTypes::OnCreate]->Method,
                scriptPointer->GetMonoObject(),
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
            if (Scripts[i].TypeInfo->Mask & ScriptEventTypes::OnDestroy)
            {
                ScriptingCore::CallMethod(
                        Scripts[i].TypeInfo->EventMethods[ScriptEventTypes::OnDestroy]->Method,
                        Scripts[i].Pointer->GetMonoObject(),
                        nullptr
                );
            }
            delete Scripts[i].Pointer; // This will free gc handle
            break;
        }
    }

    if (!found)
        return;

    Scripts.erase(Scripts.begin() + i);

    ScriptsMask = (ScriptEventTypes::ScriptEventType)0;
    for (auto script : Scripts)
        ScriptsMask = ScriptsMask | typeInfo->Mask;

    // TODO: remove component when no scripts left?
}

const std::vector<ScriptData>& ScriptComponent::GetScriptsData() const
{
    return Scripts;
}

void ScriptComponent::SetScriptsData(const std::vector<ScriptData>& scripts)
{
    Log::LogInfo("Set script data: {0}", scripts.size());
    // TODO
    Scripts = scripts;
}

ScriptPointer* ScriptComponent::GetScriptPointer(ScriptTypeInfo* typeInfo)
{
    for (auto script : Scripts)
    {
        if (script.TypeInfo == typeInfo)
            return script.Pointer;
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
    if (!(ScriptsMask & eventType))
        return;

    for (auto script : Scripts)
    {
        if (script.TypeInfo->Mask & eventType)
            ScriptingCore::CallMethod(
                    script.TypeInfo->EventMethods[eventType]->Method,
                    script.Pointer->GetMonoObject(),
                    params
            );
    }
}

void ScriptComponent::TryCallCollisionMethod(ScriptEventTypes::ScriptEventType eventType, Collision collision)
{
    void* ctorParams[1];
    ctorParams[0] = &collision.OtherEntity;
    ScriptPointer* collisionObject = ScriptingCore::CreateAPIStruct(APIStructs::Collision, ctorParams);

    void* params[1];
    params[0] = collisionObject->GetMonoObject();
    TryCallEventMethod(eventType, params);

    delete collisionObject;
}
