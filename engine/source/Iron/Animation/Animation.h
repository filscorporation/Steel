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
    Animation(Sprite** sourceSprites, uint32_t sourceSpritesCount, float animationLength);
    Animation(Sprite** sourceSprites, uint32_t sourceSpritesCount, std::vector<uint32_t>& framesDurations);

    float Length();

    ResourceID ID;
    std::string Name;
    bool Loop = false;
    std::vector<Curve> Curves;

private:
    float length;
};
