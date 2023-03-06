#include "ShaderLoader.h"
#include "Steel/Core/Log.h"

#define SHADER_VERSION_KEY "#version"

Shader* ShaderLoader::LoadShader(const FileData& fileData)
{
    std::string shaderCode = std::string(fileData.Data, fileData.Size);
    std::string::size_type pos = shaderCode.find(SHADER_VERSION_KEY, 1);

    if (pos == std::string::npos)
    {
        Log::LogError("Unable to split shader code into vertex and fragment");
        return nullptr;
    }

    auto vShaderCode = shaderCode.substr(0, pos);
    auto fShaderCode = shaderCode.substr(pos);

    return new Shader(vShaderCode.c_str(), fShaderCode.c_str());
}
