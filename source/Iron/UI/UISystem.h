#pragma once

#include "../EntityComponentSystem/ComponentSystem.h"
#include "UIElements/UIText.h"

class UISystem : public ComponentSystem<UIText>
{
public:
    void OnComponentAdded(EntityID entityID, UIText& component) override;
    void OnComponentRemoved(EntityID entityID, UIText& component) override;
    void OnEntityEnabled(EntityID entityID, UIText& component) override;
    void OnEntityDisabled(EntityID entityID, UIText& component) override;
};
