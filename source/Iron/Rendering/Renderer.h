#pragma once

#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <array>
#include "Camera.h"

class Renderer
{
public:
    static void Init(Camera* camera);
    static void Terminate();

    static void OnBeforeRender();
    static void OnAfterRender();
    static void Clear(glm::vec3 color);
    static void PrepareUIRender();
    static void DrawQuad(glm::mat4 transformation, GLuint textureID);
    static void DrawQuad(glm::mat4 transformation, GLuint textureID, glm::vec2 textureCoords[4]);

private:
    static void StartBatch();
    static void EndBatch();
    static void DrawBatchedData();
};
