#pragma once

#include "../EntityComponentSystem/Component.h"
#include "RectTransformation.h"

class UIComponent : public Component
{
public:
    explicit UIComponent(EntityID ownerEntityID);

    RectTransformation& GetRectTransform();
};
