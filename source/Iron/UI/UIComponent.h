#pragma once

#include "../Scene/Component.h"
#include "RectTransformation.h"

class UIComponent : public Component
{
public:
    explicit UIComponent(Entity* parentEntity);

    RectTransformation* GetRectTransform();
};
