#pragma once

#include <cstdint>

#include "../Rendering/QuadRenderer.h"

class UIQuadRenderer : public QuadRenderer
{
public:
    explicit UIQuadRenderer(EntityID ownerEntityID) : QuadRenderer(ownerEntityID) { }

    EntityID CustomOwner = NULL_ENTITY;
};
