#pragma once

#include <glm/glm.hpp>

#include "../EntityComponentSystem/Component.h"
#include "Material.h"
#include "MaterialPropertyBlock.h"

namespace RenderingQueue
{
    enum RenderingQueue
    {
        Opaque =      0,
        Transparent = 1,
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
    Material* Material = nullptr;
    MaterialPropertyBlock CustomProperties;

    float SortingOrder = 0;
    RenderingQueue::RenderingQueue Queue = RenderingQueue::Opaque;

    void SetDefaultQuad();
    void SetDefaultQuad(glm::vec2 pivot);
};
