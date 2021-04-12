#pragma once

#include <array>
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../Core/ResourceID.h"

struct Sprite
{
public:
    ResourceID ID;
    const char* Path;
    unsigned int TextureID;
    int Width;
    int Height;
    int PixelsPerUnit = 32;
    bool IsTransparent;

    bool IsSpriteSheet = false;
    int TileWidth;
    int TileHeight;

    void SetAsSpriteSheet(int tileWidth, int tileHeight);
    void GetTexCoord(int tileIndex, glm::vec2* texCoords);
    glm::vec3 GetRealWorldSize();
    int TilesCount();
};
