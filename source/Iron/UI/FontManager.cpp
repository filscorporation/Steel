#include <ft2build.h>
#include FT_FREETYPE_H

#include "FontManager.h"
#include "../Core/Log.h"

bool FontManager::_isInitialized = false;

static FT_Library FreeTypeLibrary;

void FontManager::Init()
{
    if (FT_Init_FreeType(&FreeTypeLibrary))
    {
        Log::LogError("Could not init FreeType library");
        return;
    }

    _isInitialized = true;
}

void FontManager::Terminate()
{
    FT_Done_FreeType(FreeTypeLibrary);
}

bool FontManager::IsInitialized()
{
    return _isInitialized;
}

Font* FontManager::FontFromPath(const char* fontPath)
{
    auto* face = new FT_Face();
    if (FT_New_Face(FreeTypeLibrary, fontPath, 0, face))
    {
        Log::LogError("Failed to load font");
        return nullptr;
    }

    auto font = new Font();
    font->name = fontPath;

    font->InitData(face);

    return font;
}
