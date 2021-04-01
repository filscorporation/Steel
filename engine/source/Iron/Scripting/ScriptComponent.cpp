#include "ScriptComponent.h"
#include "ScriptingCore.h"

void ScriptComponent::Init(const char* fullName, ScriptPointer scriptPointer)
{
    _fullName = fullName;
    _scriptPointer = scriptPointer;

    OnCreate();
}

ScriptPointer ScriptComponent::GetScriptPointer() const
{
    return _scriptPointer;
}

void ScriptComponent::OnCreate()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnCreate);
}

void ScriptComponent::OnUpdate()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnUpdate);
}

void ScriptComponent::OnLateUpdate()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnLateUpdate);
}

void ScriptComponent::OnFixedUpdate()
{
    // TODO: solve situations when collision exit will not be called (rigid body destroyed)
    if (hasCollision)
        ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnCollisionStay, collisionStay.OtherEntity);

    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnFixedUpdate);
}

void ScriptComponent::OnEnabled()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnEnabled);
}

void ScriptComponent::OnDisabled()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnDisabled);
}


void ScriptComponent::OnCollisionEnter(Collision collision)
{
    hasCollision = true;
    collisionStay = collision;

    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnCollisionEnter, collision.OtherEntity);
}

void ScriptComponent::OnCollisionExit(Collision collision)
{
    hasCollision = false;

    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnCollisionExit, collision.OtherEntity);
}

void ScriptComponent::OnMouseOver()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnMouseOver);
}

void ScriptComponent::OnMouseEnter()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnMouseEnter);
}

void ScriptComponent::OnMouseExit()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnMouseExit);
}

void ScriptComponent::OnMousePressed()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnMousePressed);
}

void ScriptComponent::OnMouseJustPressed()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnMouseJustPressed);
}

void ScriptComponent::OnMouseJustReleased()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnMouseJustReleased);
}
