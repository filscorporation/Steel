#pragma once

#include "../EntityComponentSystem/Component.h"

class UIComponent : public Component
{
public:
    explicit UIComponent(EntityID ownerEntityID) : Component(ownerEntityID) { };

protected:
    bool CheckRectTransformation(EntitiesRegistry* entitiesRegistry);
};
