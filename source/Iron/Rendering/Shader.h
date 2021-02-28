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
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void Use();
};