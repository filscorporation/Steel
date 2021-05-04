#pragma once

#include <glm/glm.hpp>

#include "../EntityComponentSystem/Component.h"

namespace RenderingQueue
{
    enum RenderingQueue
    {
        Opaque =      0,
        Text =        1,
        Transparent = 2,
    };
}

class QuadRenderer : public Component
{
public:
    explicit QuadRenderer(EntityID ownerEntityID) : Component(ownerEntityID) { }

    glm::vec4 DefaultVertices[4];
    glm::vec3 Vertices[4];
    glm::vec2 TextureCoords[4];
    glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    uint32_t TextureID = 0;

    float SortingOrder = 0;
    RenderingQueue::RenderingQueue Queue = RenderingQueue::Opaque;

    void Render();
    void SetDefaultQuad();
    void SetDefaultQuad(glm::vec2 pivot);
};
