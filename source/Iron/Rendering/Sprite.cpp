#include "Sprite.h"

void Sprite::SetAsSpriteSheet(int tileWidth, int tileHeight)
{
    IsSpriteSheet = true;
    TileWidth = tileWidth;
    TileHeight = tileHeight;
}

const float* Sprite::GetTexCoord(int tileIndex)
{
    float tw = float(TileWidth) / float(Width);
    float th = float(TileHeight) / float(Height);
    int numPerRow = Width / TileWidth;
    float tx = (tileIndex % numPerRow) * tw;
    float ty = (tileIndex / numPerRow) * th;
    auto texCoords = new float[8]{
            tx + tw, ty,
            tx + tw, ty + th,
            tx, ty,
            tx, ty + th
    };

    return texCoords;
}

int Sprite::TilesCount()
{
    return IsSpriteSheet ? Width / TileWidth * Height / TileHeight : 1;
}
