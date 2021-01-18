#pragma once

#include <vector>
#include "../Rendering/Sprite.h"
#include "../Animation/Animation.h"

class ResourcesManager
{
public:
    ~ResourcesManager();

    Sprite* LoadImage(const char* filePath);
    Sprite* GetImage(unsigned int imageID);
    void UnloadImage(unsigned int imageID);

    void AddAnimation(Animation* animation);
    Animation* GetAnimation(unsigned int animationID);
    void RemoveAnimation(unsigned int animationID);

private:
    std::vector<Sprite*> images;
    std::vector<Animation*> animations;
};
