#pragma once

#include <vector>

struct Sprite
{
public:
    unsigned int ID;
    const char* Path;
    unsigned int TextureID;
    int Width;
    int Height;

    bool IsSpriteSheet = false;
    int TileWidth;
    int TileHeight;
    void SetAsSpriteSheet(int tileWidth, int tileHeight);
    const float* GetTexCoord(int tileIndex);
};

class ResourcesManager
{
public:
    Sprite* LoadImage(const char* filePath);
    Sprite* GetImage(unsigned int imageID);

private:
    std::vector<Sprite*> images;
};
