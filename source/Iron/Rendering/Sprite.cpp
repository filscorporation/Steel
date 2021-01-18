#include "Sprite.h"

void Sprite::SetAsSpriteSheet(int tileWidth, int tileHeight)
{
    IsSpriteSheet = true;
    TileWidth = tileWidth;
    TileHeight = tileHeight;
}

std::array<float, 8> Sprite::GetTexCoord(int tileIndex)
{
    float tw = float(TileWidth) / float(Width);
    float th = float(TileHeight) / float(Height);
    int numPerRow = Width / TileWidth;
    float tx = (tileIndex % numPerRow) * tw;
    float ty = (tileIndex / numPerRow) * th;
    return {
            tx + tw, ty,
            tx + tw, ty + th,
            tx, ty,
            tx, ty + th
    };
}

int Sprite::TilesCount()
{
    return IsSpriteSheet ? Width / TileWidth * Height / TileHeight : 1;
}
