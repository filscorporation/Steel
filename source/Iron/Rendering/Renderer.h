#pragma once

#include <GLAD/glad.h>
#include <GLM/glm.hpp>
#include "Camera.h"

class Renderer
{
public:
    static void Init(Camera* camera);
    static void Terminate();

    static void OnBeforeRender();
    static void Clear(glm::vec3 color);
    static void BindTexture(GLuint textureID);
    static void DrawQuad(glm::mat4 transformation);
};
