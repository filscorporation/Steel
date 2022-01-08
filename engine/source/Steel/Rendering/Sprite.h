#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Steel/Rendering/MaterialSystem/Texture.h"

class Sprite : public Resource
{
public:
    explicit Sprite(Texture* texture);
    ~Sprite() override;

    Texture* SpriteTexture = nullptr;
    int PixelsPerUnit = 32;
    glm::vec2 Pivot = { 0.5f, 0.5f };
    bool IsTransparent = false;

    bool IsSpriteSheet = false;
    int TileWidth;
    int TileHeight;

    bool IsSliced = false;
    int SliceTopOffset = 0;
    int SliceBottomOffset = 0;
    int SliceLeftOffset = 0;
    int SliceRightOffset = 0;

    // TODO: add some kind of apply method to the pivot and ppu fields
    void SetAsNormal();
    void SetAsSpriteSheet(int tileWidth, int tileHeight);
    void SetAsSliced(int offset);
    void SetAsSliced(int topOffset, int bottomOffset, int leftOffset, int rightOffset);
    void GetTexCoord(uint32_t tileIndex, glm::vec2* texCoords) const;
    glm::vec3 GetRealWorldSize() const;
    int TilesCount() const;

    std::string GetName() const;
};
