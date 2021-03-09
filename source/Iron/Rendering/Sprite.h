#pragma once

#include <array>
#include <cstdint>
#include <glm/vec2.hpp>

#include "../Core/ResourceID.h"

struct Sprite
{
public:
    ResourceID ID;
    const char* Path;
    unsigned int TextureID;
    int Width;
    int Height;

    bool IsSpriteSheet = false;
    int TileWidth;
    int TileHeight;

    void SetAsSpriteSheet(int tileWidth, int tileHeight);
    void GetTexCoord(int tileIndex, glm::vec2* texCoords);
    int TilesCount();
};
