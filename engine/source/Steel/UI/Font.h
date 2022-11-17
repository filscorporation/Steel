#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include "Steel/Resources/Resource.h"

struct FontData;

struct Character
{
    glm::vec2 BottomLeft;
    glm::vec2 TopRight;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    uint32_t Advance;
};

struct CharactersAtlas
{
    unsigned int TextureID;
    glm::ivec2 CanvasSize;
    int MinY;
    int MaxY;
    uint32_t MaxX;
    std::unordered_map<char, Character> Characters;

    uint32_t RefCount = 0;

    uint32_t LineHeight() const
    {
        return MaxY - MinY;
    }
};

class Font : public Resource
{
    DEFINE_TYPE(Font)

public:
    Font();
    ~Font() override;

    unsigned int TextureID(uint32_t size);

    void AddSizeIfNotExists(uint32_t size);
    void FreeSize(uint32_t size);

    bool FreeAtlasOnZeroRef = true;

private:
    FontData* _data;
    std::unordered_map<uint32_t, CharactersAtlas> characters;

    void InitData(void* data);
    void LoadFromSize(uint32_t size, CharactersAtlas& atlas);
    void FreeSizeInner(uint32_t size);

    friend class FontManager;
    friend class UIText;
    friend class UIInputField;
};
