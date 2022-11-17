#include "Sprite.h"
#include "Steel/Serialization/AttributesRegistration.h"

void Sprite::RegisterType()
{
    REGISTER_TYPE(Sprite);
    REGISTER_ATTRIBUTE(Sprite, "isSliced", GetIsSliced, SetIsSliced, bool, AttributeFlags::Public, "Is sliced");
    REGISTER_ATTRIBUTE(Sprite, "sliceTopOffset", GetSliceTopOffset, SetSliceTopOffset, int, AttributeFlags::Public, "Slice top offset");
    REGISTER_ATTRIBUTE(Sprite, "sliceBottomOffset", GetSliceBottomOffset, SetSliceBottomOffset, int, AttributeFlags::Public, "Slice bottom offset");
    REGISTER_ATTRIBUTE(Sprite, "sliceLeftOffset", GetSliceLeftOffset, SetSliceLeftOffset, int, AttributeFlags::Public, "Slice left offset");
    REGISTER_ATTRIBUTE(Sprite, "sliceRightOffset", GetSliceRightOffset, SetSliceRightOffset, int, AttributeFlags::Public, "Slice right offset");
    REGISTER_ATTRIBUTE(Sprite, "pixelsPerUnit", GetPixelsPerUnit, SetPixelsPerUnit, int, AttributeFlags::Public, "Pixels per unit");
    REGISTER_ATTRIBUTE(Sprite, "pivot", GetPivot, SetPivot, glm::vec2, AttributeFlags::Public, "Pivot");
    REGISTER_ATTRIBUTE(Sprite, "isTransparent", GetIsTransparent, SetIsTransparent, bool, AttributeFlags::Public, "Is transparent");
    REGISTER_ATTRIBUTE(Sprite, "isSpriteSheet", GetIsSpriteSheet, SetIsSpriteSheet, bool, AttributeFlags::Public, "Is sprite sheet");
    REGISTER_ATTRIBUTE(Sprite, "tileWidth", GetTileWidth, SetTileWidth, int, AttributeFlags::Public, "Tile width");
    REGISTER_ATTRIBUTE(Sprite, "tileHeight", GetTileHeight, SetTileHeight, int, AttributeFlags::Public, "Tile height");
}

Sprite::Sprite(Texture* texture)
{
    Type = ResourceTypes::Sprite;
    _spriteTexture = texture;
}

Sprite::~Sprite()
{

}

void Sprite::SetAsNormal()
{
    _isSliced = false;
    _isSpriteSheet = false;
}

void Sprite::SetAsSpriteSheet(int tileWidth, int tileHeight)
{
    _isSliced = false;

    _isSpriteSheet = true;
    _tileWidth = tileWidth;
    _tileHeight = tileHeight;
}

void Sprite::SetAsSliced(int offset)
{
    _isSpriteSheet = false;

    _isSliced = true;
    _sliceTopOffset = offset;
    _sliceBottomOffset = offset;
    _sliceLeftOffset = offset;
    _sliceRightOffset = offset;
}

void Sprite::SetAsSliced(int topOffset, int bottomOffset, int leftOffset, int rightOffset)
{
    _isSpriteSheet = false;

    _isSliced = true;
    _sliceTopOffset = topOffset;
    _sliceBottomOffset = bottomOffset;
    _sliceLeftOffset = leftOffset;
    _sliceRightOffset = rightOffset;
}

void Sprite::GetTexCoord(uint32_t tileIndex, glm::vec2* texCoords) const
{
    if (!_isSpriteSheet)
    {
        texCoords[0] = glm::vec2(1.0f, 0.0f);
        texCoords[1] = glm::vec2(1.0f, 1.0f);
        texCoords[2] = glm::vec2(0.0f, 0.0f);
        texCoords[3] = glm::vec2(0.0f, 1.0f);

        return;
    }

    if (_spriteTexture == nullptr)
        return;

    float tw = (float)_tileWidth / (float)_spriteTexture->GetWidth();
    float th = (float)_tileHeight / (float)_spriteTexture->GetHeight();
    int numPerRow = (int)_spriteTexture->GetWidth() / _tileWidth;
    float tx = (float)(tileIndex % numPerRow) * tw;
    float ty = ((float)tileIndex / (float)numPerRow) * th;

    texCoords[0] = glm::vec2(tx + tw, ty);
    texCoords[1] = glm::vec2(tx + tw, ty + th);
    texCoords[2] = glm::vec2(tx, ty);
    texCoords[3] = glm::vec2(tx, ty + th);
}

glm::vec3 Sprite::GetRealWorldSize() const
{
    if (_spriteTexture== nullptr)
        return {};
    return {(float)_spriteTexture->GetWidth() / (float)_pixelsPerUnit, (float)_spriteTexture->GetHeight() / (float)_pixelsPerUnit, 1.0f};
}

int Sprite::TilesCount() const
{
    if (_spriteTexture == nullptr)
        return {};
    return _isSpriteSheet ? (int)_spriteTexture->GetWidth() / _tileWidth * (int)_spriteTexture->GetHeight() / _tileHeight : 1;
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

Texture* Sprite::GetSpriteTexture() const
{
    return _spriteTexture;
}

void Sprite::SetIsSliced(bool isSliced)
{
    _isSliced = isSliced;
}

bool Sprite::GetIsSliced() const
{
    return _isSliced;
}

void Sprite::SetSliceTopOffset(int value)
{
    _sliceTopOffset = value;
}

int Sprite::GetSliceTopOffset() const
{
    return _sliceTopOffset;
}

void Sprite::SetSliceBottomOffset(int value)
{
    _sliceBottomOffset = value;
}

int Sprite::GetSliceBottomOffset() const
{
    return _sliceBottomOffset;
}

void Sprite::SetSliceLeftOffset(int value)
{
    _sliceLeftOffset = value;
}

int Sprite::GetSliceLeftOffset() const
{
    return _sliceLeftOffset;
}

void Sprite::SetSliceRightOffset(int value)
{
    _sliceRightOffset = value;
}

int Sprite::GetSliceRightOffset() const
{
    return _sliceRightOffset;
}

void Sprite::SetPixelsPerUnit(int value)
{
    _pixelsPerUnit = value;
}

int Sprite::GetPixelsPerUnit() const
{
    return _pixelsPerUnit;
}

void Sprite::SetPivot(const glm::vec2& pivot)
{
    _pivot = pivot;
}

const glm::vec2& Sprite::GetPivot() const
{
    return _pivot;
}

void Sprite::SetIsTransparent(bool isTransparent)
{
    _isTransparent = isTransparent;
}

bool Sprite::GetIsTransparent() const
{
    return _isTransparent;
}

void Sprite::SetIsSpriteSheet(bool isSpriteSheet)
{
    _isSpriteSheet = isSpriteSheet;
}

bool Sprite::GetIsSpriteSheet() const
{
    return _isSpriteSheet;
}

void Sprite::SetTileWidth(int value)
{
    _tileWidth = value;
}

int Sprite::GetTileWidth() const
{
    return _tileWidth;
}

void Sprite::SetTileHeight(int value)
{
    _tileHeight = value;
}

int Sprite::GetTileHeight() const
{
    return _tileHeight;
}
