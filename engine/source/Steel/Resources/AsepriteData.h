#pragma once

#include "../Rendering/Sprite.h"
#include "../Animation/Animation.h"

class AsepriteData : public Resource
{
    DEFINE_TYPE(AsepriteData)

public:
    AsepriteData();

    std::string Name;
    std::vector<Sprite*> Sprites;
    std::vector<Animation*> Animations;
};
