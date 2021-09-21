#include <cstdint>

#include "ScriptComponent.h"
#include "ScriptingCore.h"

bool ScriptComponent::HasScriptType(ScriptTypeInfo* typeInfo)
{
    for (auto script : Scripts)
    {
        if (script.TypeInfo == typeInfo)
            return true;
    }

    return false;
}

void ScriptComponent::AddScript(ScriptPointer scriptPointer, ScriptTypeInfo* typeInfo)
{
    ScriptData data { scriptPointer, typeInfo };
    Scripts.emplace_back(data);
    ScriptsMask = ScriptsMask | typeInfo->Mask;

    if (typeInfo->Mask & ScriptEventTypes::OnCreate)
        ScriptingCore::CallMethod(scriptPointer, ScriptingCore::EngineCalls.callOnCreate);
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
                ScriptingCore::CallMethod(Scripts[i].Pointer, ScriptingCore::EngineCalls.callOnDestroy);
            ScriptingCore::FreeScriptHandle(Scripts[i].Pointer);
            break;
        }
    }

    if (!found)
        return;

    Scripts.erase(Scripts.begin() + i);

    ScriptsMask = (ScriptEventTypes::ScriptEventType)0;
    for (auto script : Scripts)
        ScriptsMask = ScriptsMask | typeInfo->Mask;
}

ScriptPointer ScriptComponent::GetScriptPointer(ScriptTypeInfo* typeInfo)
{
    for (auto script : Scripts)
    {
        if (script.TypeInfo == typeInfo)
            return script.Pointer;
    }

    return 0;
}

void ScriptComponent::OnCreate()
{
    CALL_IF_MASK(OnCreate)
}

void ScriptComponent::OnDestroy()
{
    CALL_IF_MASK(OnDestroy)
}

void ScriptComponent::OnUpdate()
{
    CALL_IF_MASK(OnUpdate)
}

void ScriptComponent::OnLateUpdate()
{
    CALL_IF_MASK(OnLateUpdate)
}

void ScriptComponent::OnFixedUpdate()
{
    CALL_IF_MASK(OnFixedUpdate)
}

void ScriptComponent::OnEnabled()
{
    CALL_IF_MASK(OnEnabled)
}

void ScriptComponent::OnDisabled()
{
    CALL_IF_MASK(OnDisabled)
}

void ScriptComponent::OnCollisionEnter(Collision collision)
{
    CALL_IF_MASK_PARAM(OnCollisionEnter, collision.OtherEntity)
}

void ScriptComponent::OnCollisionStay(Collision collision)
{
    CALL_IF_MASK_PARAM(OnCollisionStay, collision.OtherEntity)
}

void ScriptComponent::OnCollisionExit(Collision collision)
{
    CALL_IF_MASK_PARAM(OnCollisionExit, collision.OtherEntity)
}

void ScriptComponent::OnMouseOver()
{
    CALL_IF_MASK(OnMouseOver)
}

void ScriptComponent::OnMouseEnter()
{
    CALL_IF_MASK(OnMouseEnter)
}

void ScriptComponent::OnMouseExit()
{
    CALL_IF_MASK(OnMouseExit)
}

void ScriptComponent::OnMousePressed()
{
    CALL_IF_MASK(OnMousePressed)
}

void ScriptComponent::OnMouseJustPressed()
{
    CALL_IF_MASK(OnMouseJustPressed)
}

void ScriptComponent::OnMouseJustReleased()
{
    CALL_IF_MASK(OnMouseJustReleased)
}

void ScriptComponent::OnMouseOverUI()
{
    CALL_IF_MASK(OnMouseOverUI)
}

void ScriptComponent::OnMouseEnterUI()
{
    CALL_IF_MASK(OnMouseEnterUI)
}

void ScriptComponent::OnMouseExitUI()
{
    CALL_IF_MASK(OnMouseExitUI)
}

void ScriptComponent::OnMousePressedUI()
{
    CALL_IF_MASK(OnMousePressedUI)
}

void ScriptComponent::OnMouseJustPressedUI()
{
    CALL_IF_MASK(OnMouseJustPressedUI)
}

void ScriptComponent::OnMouseJustReleasedUI()
{
    CALL_IF_MASK(OnMouseJustReleasedUI)
}
