#pragma once

#include "UIComponent.h"
#include "UIRenderer.h"
#include "../Rendering/Sprite.h"

class UIImage : public UIComponent
{
public:
    explicit UIImage(EntityID ownerEntityID);

    void SetImage(Sprite* image);
    Sprite* GetImage();

private:
    Sprite* _image;
};
