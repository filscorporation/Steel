#include "Sprite.h"

void Sprite::SetAsSpriteSheet(int tileWidth, int tileHeight)
{
    IsSpriteSheet = true;
    TileWidth = tileWidth;
    TileHeight = tileHeight;
}

void Sprite::GetTexCoord(uint32_t tileIndex, glm::vec2* texCoords) const
{
    if (!IsSpriteSheet)
    {
        texCoords[0] = glm::vec2(1.0f, 0.0f);
        texCoords[1] = glm::vec2(1.0f, 1.0f);
        texCoords[2] = glm::vec2(0.0f, 0.0f);
        texCoords[3] = glm::vec2(0.0f, 1.0f);

        return;
    }

    float tw = (float)TileWidth / (float)Width;
    float th = (float)TileHeight / (float)Height;
    int numPerRow = Width / TileWidth;
    float tx = (float)(tileIndex % numPerRow) * tw;
    float ty = ((float)tileIndex / (float)numPerRow) * th;

    texCoords[0] = glm::vec2(tx + tw, ty);
    texCoords[1] = glm::vec2(tx + tw, ty + th);
    texCoords[2] = glm::vec2(tx, ty);
    texCoords[3] = glm::vec2(tx, ty + th);
}

glm::vec3 Sprite::GetRealWorldSize() const
{
    return glm::vec3((float)Width / (float)PixelsPerUnit, (float)Height / (float)PixelsPerUnit, 1.0f);
}

int Sprite::TilesCount() const
{
    return IsSpriteSheet ? Width / TileWidth * Height / TileHeight : 1;
}

std::string Sprite::GetName() const
{
    std::string path = std::string(Path);
    auto pos = path.find_last_of('/');
    if (pos == std::string::npos)
        pos = path.find_last_of('\\');
    if (pos == std::string::npos)
        return std::to_string(ID);

    return path.substr(pos + 1);
}
