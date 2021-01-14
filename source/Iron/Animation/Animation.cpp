#include "Animation.h"

Animation::Animation()
{

}

Animation::Animation(Sprite *sourceSprite, float animationLength)
{
    length = animationLength;
    Curve curve;
    int spriteTilesCount = sourceSprite->TilesCount();
    for (int i = 0; i < spriteTilesCount; ++i)
    {
        Keyframe keyframe(i * animationLength / spriteTilesCount, sourceSprite->ID, i);
        curve.Keyframes.push_back(keyframe);
    }
    Curves.push_back(curve);
}

Animation::Animation(Sprite **sourceSprites, int sourceSpritesCount, float animationLength)
{
    length = animationLength;
    Curve curve;
    for (int i = 0; i < sourceSpritesCount; ++i)
    {
        Keyframe keyframe(i * animationLength / sourceSpritesCount, sourceSprites[i]->ID, i);
        curve.Keyframes.push_back(keyframe);
    }
    Curves.push_back(curve);
}

float Animation::Length()
{
    return length;
}

Keyframe::Keyframe(float time, unsigned int spriteID, unsigned int tileIndex)
{
    Time = time;
    SpriteID = spriteID;
    TileIndex = tileIndex;
}
