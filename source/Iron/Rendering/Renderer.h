#pragma once

#include <GLAD/glad.h>
#include <glm/glm.hpp>

#include "RendererData.h"
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
    static void DrawQuad(QuadCache& quadCacheResult, const glm::mat4& transformation, const glm::vec4& color, GLuint textureID);
    static void DrawQuad(QuadCache& quadCacheResult, const glm::mat4& transformation, const glm::vec4& color, GLuint textureID, glm::vec2 textureCoords[4]);
    static void DrawQuadCached(const QuadCache& quadCache, const glm::vec4& color, GLuint textureID);
    static void DrawQuadCached(const QuadCache& quadCache, const glm::vec4& color, GLuint textureID, glm::vec2 textureCoords[4]);

    static int DrawCallsStats;
    static int VerticesStats;

private:
    static void StartBatch();
    static void EndBatch();
    static void DrawBatchedData();
    static int FindTextureSlot(GLuint textureID);
};
