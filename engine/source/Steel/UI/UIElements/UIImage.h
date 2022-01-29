#pragma once

#include "Steel/Rendering/MaterialSystem/Material.h"
#include "Steel/Rendering/Sprite.h"
#include "Steel/Rendering/Core/RenderContext.h"
#include "Steel/UI/RectTransformation.h"
#include "Steel/UI/UIComponent.h"

class UIImage : public UIComponent
{
    DEFINE_COMPONENT(UIImage)

public:
    explicit UIImage(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void SetDefault(EntitiesRegistry* entitiesRegistry) override;
    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;

    void Rebuild(RectTransformation& transformation, bool transformationDirty, bool sortingOrderDirty);
    void Draw(RenderContext* renderContext);

    void SetMaterial(Material* material);
    Material* GetMaterial() const;
    void SetCustomProperties(const MaterialPropertyBlock& properties);
    const MaterialPropertyBlock& GetCustomProperties();
    void SetImage(Sprite* image);
    Sprite* GetImage() const;
    void SetColor(const glm::vec4& color);
    const glm::vec4& GetColor() const;
    void SetImageTileIndex(uint32_t index);
    uint32_t GetImageTileIndex() const;
    void SetClippingLevel(short clippingLevel);

    // Will flip UVs upside down by Y (used for framebuffer texture)
    bool FlipImage = false;

protected:
    void RebuildInner(RectTransformation& transformation);

    Sprite* _image = nullptr;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Material* _material = nullptr;
    MaterialPropertyBlock _customProperties;
    short _clippingLevel = 0;

    uint32_t currentImageTileIndex = 0;

    bool isDirty = true;
    VertexBuffer vb;
    IndexBuffer ib;
    float _sortingOrder = 0;
};
