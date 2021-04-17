#pragma once

#include <glm/glm.hpp>

#include "../EntityComponentSystem/Component.h"
#include "../Rendering/RendererData.h"

class UILetterRenderer : public Component
{
public:
    explicit UILetterRenderer(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void Render();

    EntityID PreviousLetter = NULL_ENTITY;
    bool IsRendered = true;

    glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    unsigned int TextureID = 0;
    glm::vec2 TextureCoords[4] = { glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                   glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
    glm::vec4 Vertices[4];
    QuadCache Cache;
};
