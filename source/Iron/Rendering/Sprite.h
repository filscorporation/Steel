#pragma once

#include <array>
#include <cstdint>

struct Sprite
{
public:
    uint64_t ID;
    const char* Path;
    unsigned int TextureID;
    int Width;
    int Height;

    bool IsSpriteSheet = false;
    int TileWidth;
    int TileHeight;

    void SetAsSpriteSheet(int tileWidth, int tileHeight);
    std::array<float, 8> GetTexCoord(int tileIndex);
    int TilesCount();
};
