#pragma once

#include "Font.h"

struct FontData;

class FontManager
{
public:
    static void Init();
    static void Terminate();
    static bool IsInitialized();

    static Font* FontFromPath(const char* fontPath);
private:
    static bool _isInitialized;
};
