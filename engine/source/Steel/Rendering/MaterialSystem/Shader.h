#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "Steel/Resources/Resource.h"

class Shader : public Resource
{
    DEFINE_TYPE(Shader)

public:
    Shader(const char* vertexCode, const char* fragmentCode);
    virtual ~Shader();

    static Shader* FromFilePaths(const char* vertexPath, const char* fragmentPath);
    void Use() const;

    int GetUniformLocation(const std::string& name);
    int GetTextureSlot(int uniformID);

    uint32_t Program;
    bool GlobalUniformsSet = false;

private:
    std::unordered_map<std::string, int> uniformsCache;
    std::unordered_map<int, int> textureLocationsCache;
};
