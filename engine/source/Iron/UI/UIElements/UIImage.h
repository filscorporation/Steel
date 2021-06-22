#pragma once

#include "../RectTransformation.h"
#include "../UIComponent.h"
#include "../UIQuadRenderer.h"
#include "../../Rendering/Sprite.h"

class UIImage : public UIComponent
{
public:
    explicit UIImage(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void UpdateRenderer(RectTransformation& transformation, bool transformationDirty, bool sortingOrderDirty);

    void SetImage(Sprite* image);
    Sprite* GetImage();
    void SetColor(glm::vec4 color);
    glm::vec4 GetColor();
    void SetImageTileIndex(uint32_t index);
    uint32_t GetImageTileIndex();

protected:
    Sprite* _image = nullptr;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    uint32_t currentImageTileIndex = 0;
    std::vector<EntityID> _renderers;

    friend class UISystem;
};
