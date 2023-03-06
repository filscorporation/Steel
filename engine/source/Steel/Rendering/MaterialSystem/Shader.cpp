#include "Shader.h"
#include "Steel/Rendering/Core/OpenGLAPI.h"
#include "Steel/Core/Log.h"
#include "Steel/Serialization/AttributesRegistration.h"

#if defined PLATFORM_LINUX || defined PLATFORM_WINDOWS
#include <GLAD/glad.h>
#endif
#if defined PLATFORM_ANDROID
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#endif

#define MAX_TEXTURES 16

void Shader::RegisterType()
{
    REGISTER_TYPE(Shader);
}

Shader::Shader(const char* vertexCode, const char* fragmentCode)
{
    Type = ResourceTypes::Shader;

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        Log::LogError("Shader error: vertex shader compilation failed");
        Log::LogError(infoLog);
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        Log::LogError("Shader error: fragment shader compilation failed");
        Log::LogError(infoLog);
    }

    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        Log::LogError("Shader error: linking failed");
        Log::LogError(infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(this->Program);
}

void Shader::Use() const
{
    glUseProgram(this->Program);
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (uniformsCache.find(name) == uniformsCache.end())
        uniformsCache[name] = OpenGLAPI::GetUniformLocation(Program, name.c_str());

    return uniformsCache[name];
}

int Shader::GetTextureSlot(int uniformID)
{
    if (textureLocationsCache.find(uniformID) == textureLocationsCache.end())
    {
        if (textureLocationsCache.size() >= MAX_TEXTURES - 1)
        {
            Log::LogError("Too much textures in one shader, max is: {0}", MAX_TEXTURES);
            return 0;
        }
        textureLocationsCache[uniformID] = (int)textureLocationsCache.size();
    }

    return textureLocationsCache[uniformID];
}
