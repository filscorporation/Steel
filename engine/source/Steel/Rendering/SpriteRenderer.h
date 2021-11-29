#pragma once

#include <glm/vec2.hpp>

#include "Sprite.h"
#include "Core/RenderContext.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/Rendering/MaterialSystem/Material.h"
#include "Steel/Scene/Transformation.h"

class SpriteRenderer : public Component
{
    COMPONENT(SpriteRenderer)

public:
    explicit SpriteRenderer(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void Rebuild(Transformation& transformation);
    void Draw(RenderContext* renderContext);

    void SetMaterial(Material* material);
    Material* GetMaterial();
    void SetCustomProperties(const MaterialPropertyBlock& properties);
    const MaterialPropertyBlock& GetCustomProperties();
    void SetImage(Sprite* image);
    Sprite* GetImage();
    void SetImageTileIndex(uint32_t index);
    uint32_t GetImageTileIndex() const;
    glm::vec2 GetWorldSize();

private:
    void RebuildInner(Transformation& transformation);

    Sprite* _image = nullptr;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Material* _material = nullptr;
    MaterialPropertyBlock _customProperties;

    uint32_t currentImageTileIndex = 0;

    bool isDirty = true;
    VertexBuffer vb;
    IndexBuffer ib;
    float _sortingOrder = 0;
};
