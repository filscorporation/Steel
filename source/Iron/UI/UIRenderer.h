#pragma once

#include "UIComponent.h"
#include "../Rendering/Sprite.h"

class UIRenderer : public UIComponent
{
public:
    explicit UIRenderer(EntityID ownerEntityID) : UIComponent(ownerEntityID) { }

    void OnRender();

private:
    Sprite* _image; // TODO: UI data

    friend class UIImage;
};
