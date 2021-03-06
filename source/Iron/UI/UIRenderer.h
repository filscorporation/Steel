#pragma once

#include "UIComponent.h"
#include "../Rendering/Sprite.h"

class UIRenderer : public UIComponent
{
public:
    explicit UIRenderer(Entity* parentEntity) : UIComponent(parentEntity) { }

    void OnRender();

private:
    Sprite* _image; // TODO: UI data

    friend class UIImage;
};
