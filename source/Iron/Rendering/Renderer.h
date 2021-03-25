#pragma once

#include <GLAD/glad.h>
#include <glm/glm.hpp>

#include "RendererData.h"
#include "Camera.h"

namespace DrawModes
{
    enum DrawMode
    {
        Wireframe = 0,
        Normal = 1,
        Text = 2,
    };
}

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

    static void StartBatch();
    static void EndBatch();
    static void DrawBatchedData();
    static void SetDrawMode(DrawModes::DrawMode drawMode);

    static bool DrawWireframe;

    static int DrawCallsStats;
    static int VerticesStats;

private:
    static int FindTextureSlot(GLuint textureID);

    static DrawModes::DrawMode currentDrawMode;
};
