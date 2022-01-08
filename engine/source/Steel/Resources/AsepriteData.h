#pragma once

#include "../Rendering/Sprite.h"
#include "../Animation/Animation.h"

class AsepriteData : public Resource
{
public:
    AsepriteData();

    const char* Name = nullptr;
    std::vector<Sprite*> Sprites;
    std::vector<Animation*> Animations;
};
