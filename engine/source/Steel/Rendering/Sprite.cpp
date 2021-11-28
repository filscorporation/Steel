#include "Sprite.h"

Sprite::Sprite(Texture* texture)
{
    SpriteTexture = texture;
}

Sprite::~Sprite()
{

}

void Sprite::SetAsNormal()
{
    IsSliced = false;
    IsSpriteSheet = false;
}

void Sprite::SetAsSpriteSheet(int tileWidth, int tileHeight)
{
    IsSliced = false;

    IsSpriteSheet = true;
    TileWidth = tileWidth;
    TileHeight = tileHeight;
}

void Sprite::SetAsSliced(int offset)
{
    IsSpriteSheet = false;

    IsSliced = true;
    SliceTopOffset = offset;
    SliceBottomOffset = offset;
    SliceLeftOffset = offset;
    SliceRightOffset = offset;
}

void Sprite::SetAsSliced(int topOffset, int bottomOffset, int leftOffset, int rightOffset)
{
    IsSpriteSheet = false;

    IsSliced = true;
    SliceTopOffset = topOffset;
    SliceBottomOffset = bottomOffset;
    SliceLeftOffset = leftOffset;
    SliceRightOffset = rightOffset;
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

    if (SpriteTexture == nullptr)
        return;

    float tw = (float)TileWidth / (float)SpriteTexture->GetWidth();
    float th = (float)TileHeight / (float)SpriteTexture->GetHeight();
    int numPerRow = (int)SpriteTexture->GetWidth() / TileWidth;
    float tx = (float)(tileIndex % numPerRow) * tw;
    float ty = ((float)tileIndex / (float)numPerRow) * th;

    texCoords[0] = glm::vec2(tx + tw, ty);
    texCoords[1] = glm::vec2(tx + tw, ty + th);
    texCoords[2] = glm::vec2(tx, ty);
    texCoords[3] = glm::vec2(tx, ty + th);
}

glm::vec3 Sprite::GetRealWorldSize() const
{
    if (SpriteTexture == nullptr)
        return {};
    return {(float)SpriteTexture->GetWidth() / (float)PixelsPerUnit, (float)SpriteTexture->GetHeight() / (float)PixelsPerUnit, 1.0f};
}

int Sprite::TilesCount() const
{
    if (SpriteTexture == nullptr)
        return {};
    return IsSpriteSheet ? (int)SpriteTexture->GetWidth() / TileWidth * (int)SpriteTexture->GetHeight() / TileHeight : 1;
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
