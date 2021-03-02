#include <string>

#include "ScriptComponent.h"
#include "ScriptingCore.h"
#include "../Core/Log.h"

void ScriptComponent::Init(const char* fullName, int64_t scriptPointer)
{
    Log::LogInfo("Initializing script " + std::string(fullName));

    _fullName = fullName;
    _scriptPointer = scriptPointer;
}

void ScriptComponent::OnCreate()
{
    // Called from scripting api
    //ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnCreate);
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
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCalls.callOnFixedUpdate);
}
