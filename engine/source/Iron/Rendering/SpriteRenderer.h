#pragma once

#include <glm/vec2.hpp>

#include "Sprite.h"
#include "RendererData.h"
#include "../EntityComponentSystem/Component.h"
#include "../Scene/Transformation.h"
#include "Material.h"

class SpriteRenderer : public Component
{
public:
    explicit SpriteRenderer(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void UpdateRenderer(Transformation& transformation);

    void SetMaterial(Material* material);
    Material* GetMaterial();
    void SetCustomProperties(const MaterialPropertyBlock& properties);
    const MaterialPropertyBlock& GetCustomProperties();
    void SetImage(Sprite* image);
    Sprite* GetImage();
    void SetTileIndex(uint32_t tileIndex);
    uint32_t GetTileIndex() const;
    glm::vec2 GetWorldSize();

private:
    Sprite* _image = nullptr;
    glm::vec4 _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Material* _material = nullptr;
    MaterialPropertyBlock _customProperties;

    uint32_t currentImageTileIndex = 0;
};
