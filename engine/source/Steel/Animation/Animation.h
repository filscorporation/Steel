#pragma once

#include <vector>

#include "../Rendering/Sprite.h"

// TODO: make type independent keyframes
struct Keyframe
{
    Keyframe(float time, ResourceID spriteID, uint32_t tileIndex);

    float Time;
    uint32_t SpriteID;
    uint32_t TileIndex;
};

struct Curve
{
    std::vector<Keyframe> Keyframes;
};

struct Animation
{
    Animation();
    Animation(Sprite* sourceSprite, float animationLength);
    Animation(std::vector<Sprite*>& sourceSprites, float animationLength);
    Animation(std::vector<Sprite*>& sourceSprites, std::vector<uint32_t>& framesDurations);

    void EndWithNull();

    float Length() const;

    ResourceID ID = NULL_RESOURCE;
    std::string Name;
    bool Loop = false;
    std::vector<Curve> Curves;

private:
    float length;
};
