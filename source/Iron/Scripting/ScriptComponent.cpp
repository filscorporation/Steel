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

void ScriptComponent::OnUpdate()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCallsMethods.callOnUpdate);
}

void ScriptComponent::OnLateUpdate()
{
    ScriptingCore::CallMethod(_scriptPointer, ScriptingCore::EngineCallsMethods.callOnLateUpdate);
}

// TODO: other event methods
