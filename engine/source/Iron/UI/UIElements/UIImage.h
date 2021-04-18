#pragma once

#include "../UIComponent.h"
#include "../../Rendering/Sprite.h"

class UIImage : public UIComponent
{
public:
    explicit UIImage(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void UpdateRenderer(RectTransformation& transformation);

    void SetImage(Sprite* image);
    Sprite* GetImage();

protected:
    Sprite* _image;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    uint32_t currentImageTileIndex = 0;
};
