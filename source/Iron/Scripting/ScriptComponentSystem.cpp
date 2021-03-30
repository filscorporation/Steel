#include "ScriptComponentSystem.h"
#include "ScriptingCore.h"

void ScriptComponentSystem::OnComponentAdded(EntityID entityID, ScriptComponent& component)
{

}

void ScriptComponentSystem::OnComponentRemoved(EntityID entityID, ScriptComponent& component)
{
    ScriptingCore::FreeScriptHandle(component.GetScriptPointer());
}

void ScriptComponentSystem::OnEntityEnabled(EntityID entityID, ScriptComponent& component)
{
    component.OnEnabled();
}

void ScriptComponentSystem::OnEntityDisabled(EntityID entityID, ScriptComponent& component)
{
    component.OnDisabled();
}
