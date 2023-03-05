#pragma once

#include "Font.h"
#include "Steel/Resources/FileData.h"

struct FontData;

class FontManager
{
public:
    static void Init();
    static void Terminate();
    static bool IsInitialized();

    static Font* FontFromPath(const FileData& fileData);
private:
    static int _isInitialized;
};
