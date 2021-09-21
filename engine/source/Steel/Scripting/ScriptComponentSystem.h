#pragma once

#include "../EntityComponentSystem/ComponentSystem.h"
#include "ScriptingCommon.h"
#include "ScriptComponent.h"

class ScriptComponentSystem : public ComponentSystem<ScriptComponent>
{
public:
    void OnComponentAdded(EntityID entityID, ScriptComponent& component) override;
    void OnComponentRemoved(EntityID entityID, ScriptComponent& component) override;
    void OnEntityEnabled(EntityID entityID, ScriptComponent& component) override;
    void OnEntityDisabled(EntityID entityID, ScriptComponent& component) override;
};
