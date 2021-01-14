#pragma once

#include <vector>
#include "../Rendering/Sprite.h"

// TODO: make type independent keyframes
struct Keyframe
{
    Keyframe(float time, unsigned int spriteID, unsigned int tileIndex);

    float Time;
    unsigned int SpriteID;
    unsigned int TileIndex;
};

struct Curve
{
    std::vector<Keyframe> Keyframes;
};

struct Animation
{
    Animation();
    Animation(Sprite* sourceSprite, float animationLength);
    Animation(Sprite** sourceSprites, int sourceSpritesCount, float animationLength);

    float Length();

    unsigned int ID;
    const char* Name;
    bool Loop = false;
    std::vector<Curve> Curves;

private:
    float length;
};
