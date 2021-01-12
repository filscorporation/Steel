#pragma once

#include <vector>

struct Image
{
public:
    unsigned int ID;
    const char* Path;
    unsigned int TextureID;
    int Width;
    int Height;
};

class ResourcesManager
{
public:
    Image* LoadImage(const char* filePath);
    Image* GetImage(unsigned int imageID);

private:
    std::vector<Image*> images;
};
