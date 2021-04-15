#include "Animation.h"

Animation::Animation()
{

}

Animation::Animation(Sprite* sourceSprite, float animationLength)
{
    length = animationLength;
    Curve curve;
    if (sourceSprite == nullptr)
    {
        Keyframe keyframe(0.0f, NULL_RESOURCE, 0);
        curve.Keyframes.push_back(keyframe);
    }
    else
    {
        Name = sourceSprite->GetName().c_str();

        int spriteTilesCount = sourceSprite->TilesCount();
        for (int i = 0; i < spriteTilesCount; ++i)
        {
            Keyframe keyframe((float)i * animationLength / (float)spriteTilesCount, sourceSprite->ID, i);
            curve.Keyframes.push_back(keyframe);
        }
    }
    Curves.push_back(curve);
}

Animation::Animation(Sprite** sourceSprites, uint32_t sourceSpritesCount, float animationLength)
{
    if (sourceSpritesCount == 0)
    {
        length = 0.0f;
        return;
    }

    length = animationLength;
    Curve curve;
    for (int i = 0; i < sourceSpritesCount; ++i)
    {
        Keyframe keyframe((float)i * animationLength / (float)sourceSpritesCount,
                          sourceSprites[i] == nullptr ? NULL_RESOURCE : sourceSprites[i]->ID, i);
        curve.Keyframes.push_back(keyframe);
    }
    Curves.push_back(curve);
}

Animation::Animation(Sprite** sourceSprites, uint32_t sourceSpritesCount, std::vector<uint32_t>& framesDurations)
{
    if (sourceSpritesCount == 0)
    {
        length = 0.0f;
        return;
    }

    length = 0.0f;
    Curve curve;
    for (int i = 0; i < sourceSpritesCount; ++i)
    {
        Keyframe keyframe(length,sourceSprites[i] == nullptr ? NULL_RESOURCE : sourceSprites[i]->ID, i);
        curve.Keyframes.push_back(keyframe);
        length += (float)framesDurations[i] / 1000.0f;
    }
    Curves.push_back(curve);
}

float Animation::Length()
{
    return length;
}

Keyframe::Keyframe(float time, ResourceID spriteID, uint32_t tileIndex)
{
    Time = time;
    SpriteID = spriteID;
    TileIndex = tileIndex;
}
