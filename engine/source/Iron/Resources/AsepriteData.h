#pragma once

#include "../Rendering/Sprite.h"
#include "../Animation/Animation.h"

struct AsepriteData
{
public:
    ResourceID ID;
    const char* Name;
    std::vector<Sprite*> Sprites;
    std::vector<Animation*> Animations;
};
