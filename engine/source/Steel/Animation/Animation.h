#pragma once

#include <vector>

#include "Steel/Rendering/Sprite.h"

// TODO: make type independent keyframes
struct Keyframe
{
    Keyframe(float time, ResourceID spriteID, uint32_t tileIndex);

    float Time;
    ResourceID SpriteID;
    uint32_t TileIndex;
};

struct Curve
{
    std::vector<Keyframe> Keyframes;
};

class Animation : public Resource
{
public:
    Animation();
    Animation(Sprite* sourceSprite, float animationLength);
    Animation(std::vector<Sprite*>& sourceSprites, float animationLength);
    Animation(std::vector<Sprite*>& sourceSprites, std::vector<uint32_t>& framesDurations);

    void EndWithNull();

    float Length() const;

    std::string Name;
    bool Loop = false;
    std::vector<Curve> Curves;

private:
    float length;
};
