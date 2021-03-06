#pragma once

#include "UIComponent.h"
#include "UIRenderer.h"
#include "../Rendering/Sprite.h"

class UIImage : public UIComponent
{
public:
    explicit UIImage(Entity* parentEntity);

    void SetImage(Sprite* image);
    Sprite* GetImage();

private:
    Sprite* _image;
    UIRenderer* _renderer;
};
