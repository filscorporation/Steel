#pragma once

#include "FileData.h"
#include "Steel/Rendering/Sprite.h"
#include "Steel/Resources/FileLoader.h"

class PngLoader : public FileLoader
{
public:
    static Sprite* LoadImage(const FileData& fileData);
};
