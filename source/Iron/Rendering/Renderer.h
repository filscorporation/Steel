#pragma once

#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <array>
#include "Camera.h"

class Renderer
{
public:
    static void Init();
    static void Terminate();

    static void OnBeforeRender(Camera& camera);
    static void OnAfterRender();
    static void DrawScene();
    static void Clear(glm::vec3 color);
    static void PrepareUIRender();
    static void DrawUI();
    static void DrawQuad(glm::mat4 transformation, GLuint textureID);
    static void DrawQuad(glm::mat4 transformation, GLuint textureID, glm::vec2 textureCoords[4]);

    static int DrawCallsStats;
    static int VerticesStats;

private:
    static void StartBatch();
    static void EndBatch();
    static void DrawBatchedData();
};
