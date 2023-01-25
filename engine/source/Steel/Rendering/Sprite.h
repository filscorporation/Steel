#pragma once

#include "Steel/Rendering/MaterialSystem/Texture.h"

#include <array>
#include <cstdint>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Sprite : public Resource
{
    DEFINE_TYPE(Sprite)

public:
    explicit Sprite(Texture* texture);
    ~Sprite() override;

    // TODO: add some kind of apply method to the pivot and ppu fields
    void SetAsNormal();
    void SetAsSpriteSheet(int tileWidth, int tileHeight);
    void SetAsSliced(int offset);
    void SetAsSliced(int topOffset, int bottomOffset, int leftOffset, int rightOffset);
    void GetTexCoord(uint32_t tileIndex, glm::vec2* texCoords) const;
    glm::vec3 GetRealWorldSize() const;
    int TilesCount() const;

    std::string GetName() const;
    Texture* GetSpriteTexture() const;

    void SetIsSliced(bool isSliced);
    bool GetIsSliced() const;
    void SetSliceTopOffset(int value);
    int GetSliceTopOffset() const;
    void SetSliceBottomOffset(int value);
    int GetSliceBottomOffset() const;
    void SetSliceLeftOffset(int value);
    int GetSliceLeftOffset() const;
    void SetSliceRightOffset(int value);
    int GetSliceRightOffset() const;
    void SetPixelsPerUnit(int value);
    int GetPixelsPerUnit() const;
    void SetPivot(const glm::vec2& pivot);
    const glm::vec2& GetPivot() const;
    void SetIsTransparent(bool isTransparent);
    bool GetIsTransparent() const;
    void SetIsSpriteSheet(bool isSpriteSheet);
    bool GetIsSpriteSheet() const;
    void SetTileWidth(int value);
    int GetTileWidth() const;
    void SetTileHeight(int value);
    int GetTileHeight() const;

private:
    Texture* _spriteTexture = nullptr;

    int _pixelsPerUnit = 32;
    glm::vec2 _pivot = {0.5f, 0.5f };
    bool _isTransparent = false;

    bool _isSpriteSheet = false;
    int _tileWidth = 0;
    int _tileHeight = 0;

    bool _isSliced = false;
    int _sliceTopOffset = 0;
    int _sliceBottomOffset = 0;
    int _sliceLeftOffset = 0;
    int _sliceRightOffset = 0;
};
