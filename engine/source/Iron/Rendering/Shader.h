#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    uint32_t Program;
    Shader(const char* vertexCode, const char* fragmentCode);
    static Shader* FromFilePaths(const char* vertexPath, const char* fragmentPath);
    void Use();
};