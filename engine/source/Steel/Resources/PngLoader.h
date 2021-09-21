#pragma once

#include "../Rendering/Sprite.h"
#include "../Resources/FileLoader.h"

class PngLoader : public FileLoader
{
public:
    static Sprite* LoadImage(const char* filePath);
};
