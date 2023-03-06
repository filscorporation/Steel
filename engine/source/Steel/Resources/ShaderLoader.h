#pragma once

#include "FileData.h"
#include "Steel/Rendering/MaterialSystem/Shader.h"
#include "Steel/Resources/FileLoader.h"

class ShaderLoader : public FileLoader
{
public:
    static Shader* LoadShader(const FileData& fileData);
};
