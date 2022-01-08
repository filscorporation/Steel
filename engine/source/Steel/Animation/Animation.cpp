#include "Animation.h"

Animation::Animation()
{
    Type = ResourceTypes::Animation;
}

Animation::Animation(Sprite* sourceSprite, float animationLength)
{
    Type = ResourceTypes::Animation;

    length = animationLength;
    Curve curve;
    if (sourceSprite == nullptr)
    {
        Keyframe keyframe(0.0f, NULL_RESOURCE, 0);
        curve.Keyframes.push_back(keyframe);
    }
    else
    {
        Name = sourceSprite->GetName();

        int spriteTilesCount = sourceSprite->TilesCount();
        for (int i = 0; i < spriteTilesCount; ++i)
        {
            Keyframe keyframe((float)i * animationLength / (float)spriteTilesCount, sourceSprite->ID, i);
            curve.Keyframes.push_back(keyframe);
        }
    }
    Curves.push_back(curve);
}

Animation::Animation(std::vector<Sprite*>& sourceSprites, float animationLength)
{
    Type = ResourceTypes::Animation;

    if (sourceSprites.empty())
    {
        length = 0.0f;
        return;
    }

    length = animationLength;
    Curve curve;
    for (int i = 0; i < sourceSprites.size(); ++i)
    {
        Keyframe keyframe((float)i * animationLength / (float)sourceSprites.size(),
                          sourceSprites[i] == nullptr ? NULL_RESOURCE : sourceSprites[i]->ID, i);
        curve.Keyframes.push_back(keyframe);
    }
    Curves.push_back(curve);
}

Animation::Animation(std::vector<Sprite*>& sourceSprites, std::vector<uint32_t>& framesDurations)
{
    Type = ResourceTypes::Animation;

    if (sourceSprites.empty())
    {
        length = 0.0f;
        return;
    }

    length = 0.0f;
    Curve curve;
    for (int i = 0; i < sourceSprites.size(); ++i)
    {
        Keyframe keyframe(length, sourceSprites[i] == nullptr ? NULL_RESOURCE : sourceSprites[i]->ID, i);
        curve.Keyframes.emplace_back(keyframe);
        length += (float)framesDurations[i] / 1000.0f;
    }
    Curves.push_back(curve);
}

float Animation::Length() const
{
    return length;
}

void Animation::EndWithNull()
{
    if (length < 0.01f)
        return;

    for (auto& curve : Curves)
    {
        auto& lastKeyframe = curve.Keyframes[curve.Keyframes.size() - 1];
        if (std::abs(lastKeyframe.Time - length) < 0.01f)
            length += length / (float)curve.Keyframes.size();
        Keyframe keyframe(length - 0.01f, NULL_RESOURCE, 0);
        curve.Keyframes.emplace_back(keyframe);
    }
}

Keyframe::Keyframe(float time, ResourceID spriteID, uint32_t tileIndex)
{
    Time = time;
    SpriteID = spriteID;
    TileIndex = tileIndex;
}
