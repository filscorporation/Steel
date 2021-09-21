#include "ScriptComponentSystem.h"
#include "ScriptingCore.h"
#include "../UI/UIEventHandler.h"

void ScriptComponentSystem::OnComponentAdded(EntityID entityID, ScriptComponent& component)
{
    if (ComponentSystem<ScriptComponent>::Registry->HasComponent<UIEventHandler>(entityID))
    {
        ComponentSystem<ScriptComponent>::Registry->GetComponent<UIEventHandler>(entityID).EnableNotifyScripts();
    }
}

void ScriptComponentSystem::OnComponentRemoved(EntityID entityID, ScriptComponent& component)
{
    if (ComponentSystem<ScriptComponent>::Registry->HasComponent<UIEventHandler>(entityID))
    {
        ComponentSystem<ScriptComponent>::Registry->GetComponent<UIEventHandler>(entityID).DisableNotifyScripts();
    }

    component.OnDestroy();
    for (auto script : component.Scripts)
        ScriptingCore::FreeScriptHandle(script.Pointer);
}

void ScriptComponentSystem::OnEntityEnabled(EntityID entityID, ScriptComponent& component)
{
    component.OnEnabled();
}

void ScriptComponentSystem::OnEntityDisabled(EntityID entityID, ScriptComponent& component)
{
    component.OnDisabled();
}
