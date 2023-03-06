#pragma once

#include "Steel/Resources/Resource.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#ifdef PLATFORM_ANDROID
#define SHADER_VERSION "300 es"
#else
#define SHADER_VERSION "330 core"
#endif

class Shader : public Resource
{
    DEFINE_TYPE(Shader)

public:
    Shader(const char* vertexCode, const char* fragmentCode);
    ~Shader() override;

    void Use() const;

    int GetUniformLocation(const std::string& name);
    int GetTextureSlot(int uniformID);

    uint32_t Program;
    bool GlobalUniformsSet = false;

private:
    std::unordered_map<std::string, int> uniformsCache;
    std::unordered_map<int, int> textureLocationsCache;
};
