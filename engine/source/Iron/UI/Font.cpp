#include <ft2build.h>
#include FT_FREETYPE_H
#include <GLAD/glad.h>

#include "Font.h"
#include "FontManager.h"
#include "../Core/Log.h"

#define CHARACTERS_START 32
#define CHARACTERS_NUMBER 255

struct FontData
{
    FT_Face* FTFace;
};

Font::Font()
{
    _data = new FontData();
}

Font::~Font()
{
    if (_data != nullptr)
    {
        if (_data->FTFace != nullptr)
        {
            FT_Done_Face(*_data->FTFace);
            delete _data->FTFace;
        }
        delete _data;
    }
}

unsigned int Font::TextureID(uint32_t size)
{
    return characters[size].TextureID;
}

void Font::AddSizeIfNotExists(uint32_t size)
{
    if (characters.find(size) != characters.end())
    {
        characters[size].RefCount++;
        return;
    }

    CharactersAtlas atlas;
    LoadFromSize(size, atlas);
    atlas.RefCount++;
    characters[size] = atlas;
}

void Font::FreeSize(uint32_t size)
{
    if (characters.find(size) == characters.end())
        return;

    auto& atlas = characters[size];
    if (atlas.RefCount == 0)
        Log::LogWarning("Font ref count already is 0, freeing anyway");
    else
        atlas.RefCount--;

    if (atlas.RefCount == 0 && FreeAtlasOnZeroRef)
    {
        FreeSizeInner(size);
    }
}

void Font::InitData(void* data)
{
    _data->FTFace = static_cast<FT_Face*>(data);
}

void Font::LoadFromSize(uint32_t size, CharactersAtlas& atlas)
{
    auto& face = *_data->FTFace;
    FT_Set_Pixel_Sizes(face, 0, size);

    auto& glyph = face->glyph;
    uint32_t width = 0, height = 0;
    int minY = 0, maxY = 0;

    for (int i = CHARACTERS_START; i < CHARACTERS_NUMBER; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
        {
            continue;
        }

        width += (glyph->bitmap.width + 1);
        height = std::max(height, glyph->bitmap.rows);
        minY = std::min(minY, glyph->bitmap_top - (int)glyph->bitmap.rows);
        maxY = std::max(maxY, glyph->bitmap_top);
    }

    GLuint textureID;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    uint32_t x = 0;
    for (int i = CHARACTERS_START; i < CHARACTERS_NUMBER; i++)
    {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            continue;

        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, glyph->bitmap.width, glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

        Character character =
        {
            glm::vec2((float)x / (float)width, 0.0f),
            glm::vec2((float)(x + glyph->bitmap.width) / (float)width, (float)(glyph->bitmap.rows) / (float)height),
            glm::ivec2(glyph->bitmap.width, glyph->bitmap.rows),
            glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
            static_cast<uint32_t>(glyph->advance.x >> 6)
        };
        atlas.Characters[i] = character;

        x += (glyph->bitmap.width + 1);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    atlas.TextureID = textureID;
    atlas.Size = glm::ivec2(width, height);
    atlas.MinY = minY;
    atlas.MaxY = maxY;
}

void Font::FreeSizeInner(uint32_t size)
{
    auto& atlas = characters[size];

    glDeleteTextures(1, &atlas.TextureID);
    characters.erase(size);
}
