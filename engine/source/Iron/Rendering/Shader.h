#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "../Resources/ResourceID.h"

class Shader
{
public:
    ResourceID ID;
    uint32_t Program;
    Shader(const char* vertexCode, const char* fragmentCode);
    static Shader* FromFilePaths(const char* vertexPath, const char* fragmentPath);
    void Use() const;

    int GetUniformLocation(const std::string& name);
    int GetTextureSlot(int uniformID);

    bool GlobalUniformsSet = false;

private:
    std::unordered_map<std::string, int> uniformsCache;
    std::unordered_map<int, int> textureLocationsCache;
};
