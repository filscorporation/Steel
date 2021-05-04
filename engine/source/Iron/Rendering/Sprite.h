#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../Resources/ResourceID.h"

struct Sprite
{
public:
    ResourceID ID;
    const char* Path;
    uint32_t TextureID;
    uint32_t Width;
    uint32_t Height;
    int PixelsPerUnit = 32;
    glm::vec2 Pivot = { 0.5f, 0.5f };
    bool IsTransparent;

    bool IsSpriteSheet = false;
    int TileWidth;
    int TileHeight;

    // TODO: add some kind of apply method to for the pivot and ppu fields
    void SetAsSpriteSheet(int tileWidth, int tileHeight);
    void GetTexCoord(uint32_t tileIndex, glm::vec2* texCoords) const;
    glm::vec3 GetRealWorldSize() const;
    int TilesCount() const;

    std::string GetName() const;
};
