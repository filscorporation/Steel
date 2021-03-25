#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include "../Core/ResourceID.h"

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
    glm::ivec2 Size;
    uint32_t MinY;
    uint32_t MaxY;
    std::unordered_map<char, Character> Characters;
};

class Font
{
public:
    Font();
    ~Font();

    unsigned int TextureID(uint32_t size);

    void AddSizeIfNotExists(uint32_t size);

    ResourceID ID;

private:
    FontData* _data;
    const char* name;
    std::unordered_map<uint32_t, CharactersAtlas> characters;

    void InitData(void* data);
    void LoadFromSize(uint32_t size, CharactersAtlas& atlas);

    friend class FontManager;
    friend class UIText;
};
