#include "ScriptComponent.h"

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
                ScriptingCore::CallMethod(Scripts[i].ScriptPointer, ScriptingCore::EngineCalls.callOnDestroy);
            ScriptingCore::FreeScriptHandle(Scripts[i].ScriptPointer);
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
            return script.ScriptPointer;
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
    // TODO: solve situations when collision exit will not be called (rigid body destroyed)
    //if (hasCollision)
    //    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnCollisionStay, collisionStay.OtherEntity);
    // TODO: move OnCollisionStay logic to collision listener or somewhere else

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
    hasCollision = true;
    collisionStay = collision;

    CALL_IF_MASK(OnCollisionEnter)
}

void ScriptComponent::OnCollisionExit(Collision collision)
{
    hasCollision = false;

    CALL_IF_MASK(OnCollisionExit)
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
