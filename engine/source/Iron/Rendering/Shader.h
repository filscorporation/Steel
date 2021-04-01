#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLAD/glad.h>

class Shader
{
public:
    GLuint Program;
    Shader(const GLchar* vertexCode, const GLchar* fragmentCode);
    static Shader* FromFilePaths(const char* vertexPath, const char* fragmentPath);
    void Use();
};