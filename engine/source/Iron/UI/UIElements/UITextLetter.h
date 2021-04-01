#pragma once

#include "../../EntityComponentSystem/Component.h"

class UITextLetter : public Component
{
public:
    explicit UITextLetter(EntityID ownerEntityID) : Component(ownerEntityID) { };

    EntityID PreviousLetterID = NULL_ENTITY;
};
