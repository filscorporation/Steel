#pragma once

#include "Steel/EntityComponentSystem/Component.h"

class UIComponent : public Component
{
public:
    explicit UIComponent(EntityID ownerEntityID) : Component(ownerEntityID) { };
    ~UIComponent() override = default;

protected:
    bool CheckRectTransformation(EntitiesRegistry* entitiesRegistry);
};
