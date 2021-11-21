#pragma once

#include "../RectTransformation.h"
#include "../UIComponent.h"
#include "../UIQuadRenderer.h"
#include "../../Rendering/Material.h"
#include "../../Rendering/Sprite.h"

class UIImage : public UIComponent
{
    COMPONENT(UIImage)

public:
    explicit UIImage(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;

    void UpdateRenderer(RectTransformation& transformation, bool transformationDirty, bool sortingOrderDirty);

    void SetMaterial(Material* material);
    Material* GetMaterial();
    void SetCustomProperties(const MaterialPropertyBlock& properties);
    const MaterialPropertyBlock& GetCustomProperties();
    void SetImage(Sprite* image);
    Sprite* GetImage();
    void SetColor(glm::vec4 color);
    glm::vec4 GetColor();
    void SetImageTileIndex(uint32_t index);
    uint32_t GetImageTileIndex();
    void SetClippingLevel(short clippingLevel);

    // Will flip UVs upside down by Y (used for framebuffer texture)
    bool FlipImage = false;

protected:
    Sprite* _image = nullptr;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Material* _material = nullptr;
    MaterialPropertyBlock _customProperties;
    short _clippingLevel = 0;

    uint32_t currentImageTileIndex = 0;
    std::vector<EntityID> _renderers;
};
